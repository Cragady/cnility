#ifndef EXTENSIONS_FILE_CONVERSION_UTF8_UTILS_HPP_
#define EXTENSIONS_FILE_CONVERSION_UTF8_UTILS_HPP_

#include <cstddef>
#include <cstdint>

namespace utf8_utils {

uint32_t parse_utf8(const char *str, size_t size);
uint32_t parse_utf8_us(const char *str);
void sanitize_utf8_for_html(char *str, size_t max_len);

} // namespace utf8_utils

#endif
