#include <cstdio>
#include <iostream>
#include "my_stack.h"

void printFirstK(int k) {
  Stack<int> st;
  for (int i = 0; i < k; ++i) {
    st.push(i);
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
  std::cout << "Hello, user of my stack!" << std::endl;

  try {
    printFirstK(1000);
  } catch (StackException stackException) {
    std::cerr << stackException << '\n';
  }
  return 0;
}