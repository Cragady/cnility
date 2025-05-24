#include "StringRunner.hpp"

#include <cstddef>
#include <iostream>

StringRunner::StringRunner(const char *str) {
  str_master_ = str;
  str_ = (char *)str;
}

void StringRunner::LogStr() {
  std::cout << str_ << std::endl;
}

void StringRunner::ReadStr(char *dest, size_t size) {
  size_t dest_size = 0;

  while (*str_ && dest_size < size) {
    *dest++ = *str_++;
    dest_size++;
  }
}
