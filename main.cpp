#include <cstdio>
#include <iostream>
#include "my_stack.h"
#include "crash_test.h"
//#define DEBUG
//#include "logipamar_stack.h"

void printFirstK(int k) {
  Stack<int> st;
  for (int i = 0; i < k; ++i) {
    st.push(i);
    if (i % 10 == 3) {
      st.dump();
    }
  }
  for (int i = 0; i < k; ++i) {
    std::cout << st[i] << ' ';
  }
  std::cout << '\n';
  for (int i = 0; i < k; ++i) {
    st.pop();
  }
}

void copyStack() {
  Stack<std::string> st;
  st.push("hello, comrade!");
  st.push("stack overflow");
  std::cout << st[0] << ' ' << st[1] << '\n';
  Stack<std::string> st2 = st;
  std::cout << st2[0] << ' ' << st2[1] << '\n';
}

int main() {
  try {
    copyStack();
    nullPtrTest1<Stack<int>>();
    nullPtrTest2<Stack<int>>();
    okTest<Stack<int>>();
    hashSumTest1<Stack<int>>();
    canaryTest1<Stack<int>>();
    canaryTest2<Stack<int>>();
    stringCopyTest<Stack<std::string>>();
    //stringMoveTest<Stack<std::string>>();
  } catch (StackException stackException) {
    std::cerr << stackException << '\n';
  }
  return 0;
}