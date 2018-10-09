#ifndef STACK_EXCEPTION_H
#define STACK_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

struct StackException : public std::exception {
  std::string message;
  std::string function_name;

  StackException(const std::string& message = "", const std::string& function_name = ""):
    message(message), function_name(function_name) {}
};

struct IncorrectArgumentException : public StackException {
  IncorrectArgumentException(const std::string& message, const std::string& function_name = ""):
    StackException(message, function_name) {}
};

struct OutOfRangeException : public StackException {
  OutOfRangeException(const std::string& message, const std::string& function_name = ""):
    StackException(message, function_name) {}
};

struct EmptyStackException : public StackException {
  EmptyStackException(const std::string& message, const std::string& function_name = ""):
    StackException(message, function_name) {}
};

struct CanaryException : public StackException {
  CanaryException(const std::string& message, const std::string& function_name = ""):
    StackException(message, function_name) {}
};

struct HashSumException : public StackException {
  HashSumException(const std::string& message, const std::string& function_name = ""):
    StackException(message, function_name) {}
};

struct ParamsPoisonedException : public StackException {
  ParamsPoisonedException(const std::string& message, const std::string& function_name = ""):
    StackException(message, function_name) {}
};

struct IncorrectPointerException : public StackException {
  IncorrectPointerException(const std::string& message, const std::string& function_name = ""):
    StackException(message, function_name) {}
};

std::ostream& operator<<(std::ostream& os, const StackException& iaexception) {
  os << "!!! Exception: " << iaexception.message;
  if (!iaexception.function_name.empty()) {
    os << "(" << iaexception.function_name << ")";
  }
  os << '\n';
  return os;
}


#endif //STACK_EXCEPTION_H
