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

  std::cerr << "OK\n";
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
  std::cerr << "OK\n";
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
    std::cout << "I try to debug\n";
    Container st;
    st.dump();
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

template<class Container>
void stringCopyTest() {
  try {
    std::cout << "I try to debug\n";
    Container st;
    st.push("hello");
    st.push(",");
    st.push("world!");

    Container st2 = st;

    const std::string *cptr = &st2[1];
    std::string *ptr = const_cast<std::string *>(cptr);
    *(ptr + 2) = "azaza ahaha";
    //st2.dump();

    Container st3 = st2;

  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

template<class Container>
void stringMoveTest() {
  try {
    std::cout << "I try to debug\n";
    Container st;
    st.push("hello");
    st.push(",");
    st.push("world!");

    Container st2 = std::move(st);

    const std::string *cptr = &st2[1];
    std::string *ptr = const_cast<std::string *>(cptr);
    *(ptr + 2) = "canary should be destroyed";
    //st2.dump();

    Container st3 = std::move(st2);

  } catch (StackException& exc) {
    std::cerr << exc;
  }
}

void copyStack() {
  try {
    Stack<std::string> st;
    st.push("hello, comrade!");
    st.push("stack overflow");
    std::cout << st[0] << ' ' << st[1] << '\n';
    Stack<std::string> st2 = st;
    //std::cout << st2[0] << ' ' << st2[1] << '\n';
  } catch (const StackException& exc) {
    std::cerr << exc;
  }
}

#endif //STACK_CRASH_TEST_H
