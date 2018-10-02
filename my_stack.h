//
// Created by mike on 02.10.18.
//

#ifndef STACK_MY_STACK_H
#define STACK_MY_STACK_H

#include <cstdint>

#include "exception.h"

template<class T>
class Stack {
 private:
  const size_t EXPANSION_COEFF = 2;
  const size_t SHRINKAGE_COEFF = 2;
  const double MAX_LOAD_FACTOR = 0.5;
  const double MIN_LOAD_FACTOR = 0.25;
  const size_t MIN_CAPACITY = 8;
#ifndef NDEBUG
  const size_t CANARY_SIZE = 4;
  const uint32_t CANARY_INIT_VALUE = 1983776228;
#else
  const size_t CANARY_SIZE = 0;
#endif


  char* bytes_{nullptr};
  size_t capacity_{0};
  size_t item_count_{0};
  char* items_begin_{nullptr};
#ifndef NDEBUG
  size_t hash_sum_{0};
  std::hash<T> hasher_;

  uint32_t* getCanaryPtr1() const {
    return reinterpret_cast<uint32_t*>(bytes_);
  }

  uint32_t* getCanaryPtr2() const {
    return reinterpret_cast<uint32_t*>(items_begin_ + capacity_ * sizeof(T));
  }

  void initCanaries() {
    *getCanaryPtr1() = CANARY_INIT_VALUE;
    *getCanaryPtr2() = CANARY_INIT_VALUE;
  }
#endif
  const T* getElementPtr(size_t pos) const {
    return reinterpret_cast<T*>(items_begin_ + pos * sizeof(T));
  }

  T* getElementPtr(size_t pos) {
    return reinterpret_cast<T*>(items_begin_ + pos * sizeof(T));
  }

  void setCapacity(size_t new_capacity) {
    char* new_bytes = new char[new_capacity * sizeof(T) + 2 * CANARY_SIZE + 1];
    T* cur_item = reinterpret_cast<T*>(items_begin_);
    T* new_item = reinterpret_cast<T*>(new_bytes + CANARY_SIZE);

    for (size_t item_id = 0; item_id < item_count_; ++item_id, ++cur_item, ++new_item) {
      *new_item = *cur_item;
    }

    std::swap(new_bytes, bytes_);
    items_begin_ = bytes_ + CANARY_SIZE;
    capacity_ = new_capacity;
    initCanaries();
    delete[] new_bytes;
  }

  void extend() {
    setCapacity(capacity_ * 2);
  }

  void shrink() {
    setCapacity(capacity_ / 2);
  }

 public:
  Stack() {
    bytes_ = new char[MIN_CAPACITY * sizeof(T) + 2 * CANARY_SIZE + 1];
    items_begin_ = bytes_ + CANARY_SIZE;
    capacity_ = MIN_CAPACITY;
    initCanaries();
  }

  T& operator[](size_t pos) {
    if (pos >= item_count_) {
      throw OutOfRangeException("the argument is more than the number of elements", __PRETTY_FUNCTION__);
    }
    return *getElementPtr(pos);
  }

  const T& operator[](size_t pos) const {
    if (pos >= item_count_) {
      throw OutOfRangeException("the argument is more than the number of elements", __PRETTY_FUNCTION__);
    }
    return *getElementPtr(pos);
  }

  void push(const T& value) {
    if ((item_count_ + 1) > MAX_LOAD_FACTOR * capacity_) {
      extend();
    }
#ifndef NDEBUG
    checkCanaries(__PRETTY_FUNCTION__);
    checkHashSum(__PRETTY_FUNCTION__);
#endif
    *getElementPtr(item_count_) = value;
    ++item_count_;
#ifndef NDEBUG
    hash_sum_ = calcHashSum();
#endif
  }

  void pop() {
    if (item_count_ == 0) {
      throw EmptyStackException("", __PRETTY_FUNCTION__);
    }
    if (capacity_ > MIN_CAPACITY && (item_count_ - 1) < MIN_LOAD_FACTOR * capacity_) {
      shrink();
    }
#ifndef NDEBUG
    checkCanaries(__PRETTY_FUNCTION__);
    checkHashSum(__PRETTY_FUNCTION__);
#endif
    --item_count_;
#ifndef NDEBUG
    hash_sum_ = calcHashSum();
#endif
  }

  T& top() {
    return *this[item_count_ - 1];
  }

  const T& top() const {
    return *this[item_count_ - 1];
  }

#ifndef NDEBUG
  const size_t MODULO = 1e12 + 7;
  const size_t BASE = 15487469;

  size_t calcHashSum() const {
    size_t res = 0;

    for (size_t item_id = 0; item_id < item_count_; ++item_id) {
      res = (res * BASE + hasher_(*getElementPtr(item_id))) % MODULO;
    }
    return res;
  }

  void checkCanaries(const std::string& func_name = "") {
    if (*getCanaryPtr1() != CANARY_INIT_VALUE) {
      throw CanaryException("problem with first canary", func_name);
    }
    if (*getCanaryPtr2() != CANARY_INIT_VALUE) {
      throw CanaryException("problem with second canary", func_name);
    }
  }

  void checkHashSum(const std::string& func_name = "") {
    if (hash_sum_ != calcHashSum()) {
      throw HashSumException("", func_name);
    }
  }

  bool check() const {
    try {
      checkCanaries();
      checkHashSum();
      return true;
    } catch (StackException) {
      return false;
    }
  }

  void dump() {
    std::cout << "azaza ahaha\n";
  }
#endif

  ~Stack() {
    for (size_t item_id = 0; item_id < item_count_; ++item_id) {
      getElementPtr(item_id)->~T();
    }
    delete[] bytes_;
  }
};

#endif //STACK_MY_STACK_H
