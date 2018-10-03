//
// Created by mike on 02.10.18.
//

#ifndef STACK_MY_STACK_H
#define STACK_MY_STACK_H

#include <cstdint>
#include <utility>

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

  void copyItems(const char* from, char* to) {
    const T* cur_item = reinterpret_cast<const T*>(from + CANARY_SIZE);
    T* new_item = reinterpret_cast<T*>(to + CANARY_SIZE);

    for (size_t item_id = 0; item_id < item_count_; ++item_id, ++cur_item, ++new_item) {
      *new_item = *cur_item;
    }
  }

  void setCapacity(size_t new_capacity) {
    char* new_bytes = new char[new_capacity * sizeof(T) + 2 * CANARY_SIZE + 1];

    copyItems(bytes_, new_bytes);
    destroy();
    std::swap(new_bytes, bytes_);
    items_begin_ = bytes_ + CANARY_SIZE;
    capacity_ = new_capacity;
#ifndef NDEBUG
    initCanaries();
#endif
  }

  void extend() {
    setCapacity(capacity_ * 2);
  }

  void shrink() {
    setCapacity(capacity_ / 2);
  }

  void destroy() {
    for (size_t item_id = 0; item_id < item_count_; ++item_id) {
      getElementPtr(item_id)->~T();
    }
    delete[] bytes_;
  }

  template <class StackRef>
  void copyParameters(const StackRef&& another) {
    capacity_ = another.capacity_;
    item_count_ = another.item_count_;
    items_begin_ = another.items_begin_;
#ifndef NDEBUG
    initCanaries();
    hash_sum_ = another.hash_sum_;
    hasher_ = another.hasher_;
#endif
  }

#ifndef NDEBUG
  bool checkCanaries() const {
    bool all_fine = true;

    std::cerr << "canary 1 = " << ' ' << *getCanaryPtr1() << '\n';
    if (*getCanaryPtr1() == CANARY_INIT_VALUE) {
      std::cerr << "ok\n";
    } else {
      std::cerr << "incorrect value! should be " << CANARY_INIT_VALUE << '\n';
      all_fine = false;
    }
    std::cerr << "canary 2 = " << ' ' << *getCanaryPtr2() << '\n';
    if (*getCanaryPtr2() == CANARY_INIT_VALUE) {
      std::cerr << "ok\n";
    } else {
      std::cerr << "incorrect value! should be " << CANARY_INIT_VALUE << '\n';
      all_fine = false;
    }
    return all_fine;
  }

  bool checkHashSum() const {
    std::cerr << "saved hash sum: " << hash_sum_ << '\n';
    size_t correct_hash_sum = calcHashSum();
    if (hash_sum_ == correct_hash_sum) {
      std::cerr << "ok\n";
      return true;
    } else {
      std::cerr << "incorrect value! should be " << correct_hash_sum << '\n';
      return false;
    }
  }

  void printItems() const {
    std::cerr << "stack_begin_address: " << &items_begin_ << '\n';
    std::cerr << "item count: " << item_count_ << '\n';
    std::cerr << "items: ";
    for (size_t item_id = 0; item_id < item_count_; ++item_id) {
      std::cerr << *getElementPtr(item_id) << ' ';
    }
    std::cerr << '\n';
  }

  const size_t MODULO = 1e12 + 7;
  const size_t BASE = 15487469;

  size_t calcHashSum() const {
    size_t res = 0;

    for (size_t item_id = 0; item_id < item_count_; ++item_id) {
      res = (res * BASE + hasher_(*getElementPtr(item_id))) % MODULO;
    }
    return res;
  }

  void assertCanaries(const std::string& func_name = "") const {
    if (*getCanaryPtr1() != CANARY_INIT_VALUE) {
      throw CanaryException("problem with first canary", func_name);
    }
    if (*getCanaryPtr2() != CANARY_INIT_VALUE) {
      throw CanaryException("problem with second canary", func_name);
    }
  }

  void assertHashSum(const std::string& func_name = "") const {
    if (hash_sum_ != calcHashSum()) {
      throw HashSumException("", func_name);
    }
  }

  void assertCorrectness() const {
    assertCanaries();
    assertHashSum();
  }
#endif

 public:
  Stack() {
    bytes_ = new char[MIN_CAPACITY * sizeof(T) + 2 * CANARY_SIZE + 1];
    items_begin_ = bytes_ + CANARY_SIZE;
    capacity_ = MIN_CAPACITY;
#ifndef NDEBUG
    initCanaries();
#endif
    std::cerr << "stack was created\n";
  }

  Stack(Stack&& another) {
    bytes_ = another.bytes_;
    copyParameters(another);
    another.bytes_ = nullptr;
  }

  Stack(const Stack& another) {
    bytes_ = another.bytes_;
    copyParameters(std::forward<const Stack>(another));
    bytes_ = new char[2 * CANARY_SIZE + item_count_ * sizeof(T) + 1];
    copyItems(another.bytes_, bytes_);
  }

  T& operator=(Stack&& another) {
    if (this == &another) {
      return *this;
    }
#ifndef NDEBUG
    assertCorrectness();
#endif
    destroy();
    bytes_ = another.bytes_;
    copyParameters(another);
    another.bytes_ = nullptr;
    return *this;
  }

  T& operator=(const Stack& another) {
    if (this == &another) {
      return *this;
    }
#ifndef NDEBUG
    assertCorrectness();
#endif
    destroy();
    copyParameters(another);
    bytes_ = new char[2 * CANARY_SIZE + item_count_ * sizeof(T) + 1];
    copyItems(another.bytes_, bytes_);
    return *this;
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
    assertCorrectness();
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
    assertCorrectness();
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
  bool check() const {
    try {
      assertCanaries();
      assertHashSum();
      return true;
    } catch (StackException) {
      return false;
    }
  }

  void dump() const {
    std::cerr << "__________________________________________\n";
    bool correct_canaries = checkCanaries();
    printItems();
    bool correct_hash_sum = checkHashSum();
    bool total_correct = correct_canaries || correct_hash_sum;

    if (total_correct) {
      std::cerr << "All right\n";
      return;
    }

    std::cerr << "A problem occured\n";
    if (!correct_canaries) {
      std::cerr << "canaries were overwritten\n";
    }
    if (!correct_hash_sum) {
      std::cerr << "control hash sum don't match\n";
    }
  }
#endif

  ~Stack() {
    destroy();
  }
};

#endif //STACK_MY_STACK_H
