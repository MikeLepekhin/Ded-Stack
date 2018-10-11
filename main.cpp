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
      st.dump(__PRETTY_FUNCTION__);
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

int main() {
  try {
    //stringMoveTest<Stack<std::string>>();
    nullPtrTest1<Stack<int>>();
    /*nullPtrTest2<Stack<int>>();
    okTest<Stack<int>>();
    hashSumTest1<Stack<int>>();
    canaryTest1<Stack<int>>();
    canaryTest2<Stack<int>>();*/
  } catch (StackException& stackException) {
    std::cerr << stackException << '\n';
  }
  return 0;
}