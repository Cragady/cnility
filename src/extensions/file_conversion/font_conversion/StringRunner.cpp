#include "StringRunner.hpp"
#include "Endian.hpp"

#include <climits>
#include <cstddef>
#include <iostream>
#include <string>

StringRunner::StringRunner(std::string str) : endian_(EndianOrder::Big) {
  str_ = str;
}

// WARN: only use this for simple single variables
size_t StringRunner::RevReadStr_(char *dest, size_t size) {
  size_t dest_size = 0;
  size_t source_size = str_.size();
  size_t start_pos = str_pos_;
  char *str = (char *)str_.c_str();
  size_t byte_chunk_offset = size / CHAR_BIT;

  size_t target = str_pos_;
  size_t desired_start = str_pos_ + size;

  if (desired_start > str_.size()) {
    LogStringBoundsReadError("In StringRunner::RevReadStr_");
    desired_start = str_.size();
  }
  size_t pos = desired_start;

  str += pos - 1;

  while (
    (dest_size + start_pos) < source_size &&
    str_pos_ < source_size &&
    pos > target &&
    dest_size < size
  ) {
    *dest++ = *str--;
    pos--;
    dest_size++;
    str_pos_++;
  }
  return dest_size;
}

void StringRunner::LogStr() {
  std::cout << str_ << std::endl;
  // NOTE: The below would be needed to get the equivalent output as above
  // there are a ton of null characters in the files
  // size_t logged = 0;
  // size_t source_size = str_.size();
  // char *str = (char *)str_.c_str();
  // while (logged < source_size) {
  //   std::cout << *str++;
  //   logged++;
  // }
  // std::cout << std::endl;
}

void StringRunner::LogStrFromInternalPos() {
  char *str = (char *)str_.c_str();

  str += str_pos_;
  size_t logged = str_pos_;
  size_t source_size = str_.size();
  while (logged < source_size) {
    std::cout << *str++;
    logged++;
  }
  std::cout << std::endl;
}

size_t StringRunner::ReadStr(char *dest, size_t size, bool allow_read_rev) {
  if (allow_read_rev && endian_.should_reverse) return RevReadStr_(dest, size);
  size_t dest_size = 0;
  size_t start_pos = str_pos_;
  size_t source_size = str_.size();
  char *str = (char *)str_.c_str();
  str += str_pos_;

  while (
    (dest_size + start_pos) < source_size &&
    str_pos_ < source_size &&
    dest_size < size
  ) {
    *dest++ = *str++;
    dest_size++;
    str_pos_++;
  }

  if (dest_size != size) {
    LogStringBoundsReadError("In StringRunner::ReadStr");
  }

  return dest_size;
}

void StringRunner::ReadStrStatic(char *dest, size_t size) {
  size_t size_written = ReadStr(dest, size);
  str_pos_ -= size_written;
}

void StringRunner::PlacePos(size_t pos) {
  str_pos_ = pos;
}

void StringRunner::SetTag(uint32_t tag_nums) {
  uint32_t filter = 0xff;
  for (size_t i = 0; i < 4; i++) {
    tag[(4 - 1) - i] = tag_nums & filter;
    tag_nums >>= 8;
  }
  // TODO: get rid of magic
  tag[4] = '\0';
}

void StringRunner::LogStringBoundsReadError(const char *msg) {
  std::cerr << "Attempted to read past bounds of string!" << std::endl;
  std::cerr << "\tTable that had err: " << tag << std::endl;
  if (msg) std::cerr << "\tMessage: " << msg << std::endl;
}
