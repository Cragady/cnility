#include "Endian.hpp"

#include <cstdint>
#include <cstddef>
#include <iostream>

Endian::Endian(EndianOrder target_arg) {
  int num = 1;
  char *ptr = reinterpret_cast<char *>(&num);
  target = target_arg;

  if (*ptr == 1) {
    host = EndianOrder::Little;
  } else {
    host = EndianOrder::Big;
  }
}

uint16_t Endian::Swap16(uint16_t bytes) {
  num_bytes = 2;
  return (uint16_t)Swap(bytes);
}

uint32_t Endian::Swap32(uint32_t bytes) {
  num_bytes = 4;
  return (uint32_t)Swap(bytes);
}

uint64_t Endian::Swap64(uint64_t bytes) {
  num_bytes = 8;
  return Swap(bytes);
}

uint64_t Endian::Swap(uint64_t bytes) {
  if (host == target) return bytes;
  uint64_t dest = 0;

  for (size_t i = 0; i < num_bytes; i++) {
    dest |= (bytes & filter_byte);
    if (i < num_bytes - 1) dest <<= byte_width;
    bytes >>= byte_width;
  }


  return dest;
}
