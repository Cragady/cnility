#include <cstdio>
#include <cstring>
#include <string>

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

void sanitize_utf8_for_html(char *str, size_t max_len) {

  std::string new_str(str);
  std::string replacement;

  for (size_t i = 0; i < new_str.length(); i++) {
    char c = new_str[i];
    switch (c) {
      case '<': replacement += "&lt;"; break;
      case '>': replacement += "&gt;"; break;
      case '&': replacement += "&amp;"; break;
      case '"': replacement += "&quot;"; break;
      case '\'': replacement += "&apos;"; break;
      // case '\'': replacement += "&#39;"; break;
      case '`': replacement += "&#96"; break;
      default: replacement += c;
    }
  }

  snprintf(str, max_len - 2, "%s", replacement.c_str());
  str[max_len - 1] = '\0';

}

} // namespace utf8_utils
