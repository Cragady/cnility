#include <cstring>

#include "utf8_utils.hpp"

namespace utf8_utils {

uint32_t parse_utf8(const char *str, size_t size) {
  unsigned char *bytes = (unsigned char *)str;

  uint32_t num = 0;
  for (size_t i = 0; i < size; i++) {
    int clear_bit = 0x3f;
    int offset = (size - 1) * 6 - i * 6;
    if (i == 0) {
      clear_bit = 0x0f;
    }
    num |= ((bytes[i] & clear_bit) << offset);
  }

  return num;
}

uint32_t parse_utf8_us(const char *str) {
  size_t str_len = strlen(str);

  return parse_utf8(str, str_len);
}

} // namespace utf8_utils
