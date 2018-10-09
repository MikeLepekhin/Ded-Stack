#ifndef STACK_CRASH_TEST_H
#define STACK_CRASH_TEST_H

#include <iostream>

#include "exception.h"

template<class Container>
void nullPtrTest1() {
  try {
    Container st;
    for (int i = -10; i <= 10; ++i) {
      st.push(i);
    }
    int *st_ptr = reinterpret_cast<int*>(&st);
    *(st_ptr + 20) = 0;
    std::cout << st[0] << '\n';
    st.dump();
  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

template<class Container>
void nullPtrTest2() {
  try {
    Container st;
    for (int i = -10; i <= 10; ++i) {
      st.push(i);
    }
    int *st_ptr = reinterpret_cast<int*>(&st);
    *(st_ptr + 20) = 0;
    std::cout << st[10] << '\n';
    st.dump();
  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

template<class Container>
void okTest() {
  try {
    Container st;
    for (int iter = 0; iter < 15; ++iter) {
      for (int i = -1000; i <= 1000; ++i) {
        st.push(i);
      }
      for (int i = 0; i < 10; ++i)
        std::cout << st[i] << ' ';
      std::cout << '\n';

      for (int i = -1000; i <= 1000; ++i) {
        st.pop();
      }
    }
    std::cout << "Everything is ok\n";

  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

template<class Container>
void hashSumTest1() {
  try {
    Container st;
    for (int i = 0; i < 100; ++i) {
      st.push(i);
    }
    const int* cptr = &st[0];

    int* ptr = const_cast<int*>(cptr);
    *(ptr + 10) = 17;

    for (int i = 0; i < 100; ++i) {
      st.pop();
    }
  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

template<class Container>
void canaryTest1() {
  try {
    Container st;
    for (int i = 0; i < 100; ++i) {
      st.push(i);
    }
    const int* cptr = &st[0];

    int* ptr = const_cast<int*>(cptr);
    *(ptr - 1) = 0;

    for (int i = 0; i < 100; ++i) {
      st.pop();
    }
  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

template<class Container>
void canaryTest2() {
  try {
    Container st;
    for (int i = 0; i < 100; ++i) {
      st.push(i);
    }
    const int* cptr = &st[0];

    int* ptr = const_cast<int*>(cptr);

    for (int i = 0; i < 100; ++i) {
      *(ptr + i) = 228;
    }

    for (int i = 100; i < 257; ++i) {
      *(ptr + i) = 0;
    }

    for (int i = 0; i < 100; ++i) {
      st.pop();
    }
  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

#endif //STACK_CRASH_TEST_H
