#include <iostream>
#include <cstddef>
#include <cstdio>

#include "CharNumCorrection.hpp"
#include "utf8_utils.hpp"

bool CharNumCorrection::BasicConversion() {
  if (char_to_convert >= _char_specials || char_to_convert <= _chars_start) return false;
  char_correction = char_to_convert - _char_distance;
  return true;
}

bool CharNumCorrection::SpecialCaseConversion(bool log_case) {
  bool converted = false;
  size_t pos = 0;

  switch (char_to_convert) {
    case 0xe669:
      char_correction = utf8_utils::parse_utf8_us("“"); // u+201c
      return true;
    case 0xe66f:
      char_correction = utf8_utils::parse_utf8_us("–"); // u+2013
      return true;
    case 0xe674:
      char_correction = utf8_utils::parse_utf8_us("•"); // u+2022
      return true;
    case 0xe677:
      char_correction = utf8_utils::parse_utf8_us("”"); // u+201d
      return true;
    case 0xe689:
      char_correction = utf8_utils::parse_utf8_us("—"); // u+2014
      return true;
    case 0xe6a0:
      char_correction = utf8_utils::parse_utf8_us("°"); // u+00b0
      return true;
    case 0xe6a4:
      char_correction = utf8_utils::parse_utf8_us("-"); // u+002d
      return true;
    case 0xe6a6:
      char_correction = utf8_utils::parse_utf8_us("×"); // u+00d7
      return true;
    // TODO: verify that glyphs for the uncovered cases are just kerning for
    // the font
    // fi, fl, ff, and ffi is in the `Alphabetic Presentation Forms` in the unicode table
    case 0xe66d: // fi
      if (log_case) std::cout << "f portions" << std::endl;
      correction[pos++] = 'f';
      correction[pos++] = 'i';
      correction[pos] = '\0';
      uncovered_case = true;
      break;
    case 0xe66e: // fl
      if (log_case) std::cout << "f portions" << std::endl;
      correction[pos++] = 'f';
      correction[pos++] = 'l';
      correction[pos] = '\0';
      uncovered_case = true;
      break;
    case 0xe70f: // ff
      if (log_case) std::cout << "f portions" << std::endl;
      correction[pos++] = 'f';
      correction[pos++] = 'f';
      correction[pos] = '\0';
      uncovered_case = true;
      break;
    case 0xe710: // ffi
      if (log_case) std::cout << "f portions" << std::endl;
      log_out = true;
      correction[pos++] = 'f';
      correction[pos++] = 'f';
      correction[pos++] = 'i';
      correction[pos] = '\0';
      uncovered_case = true;
      break;
    default:
      // NOTE: we pass through regularly encoded utf-8 here.
      char_correction = char_to_convert;
      converted = true;
      break;
  }

  if (char_to_convert >= 0xe717 && char_to_convert <= 0xe72f) {
    // TODO: maybe wrap these in a span to preserve font sizing
    char_correction = char_to_convert - _char_special_capitals_distance;
    converted = true;
  }

  return converted;
}

void CharNumCorrection::Conversion() {
  ZeroString();
  parse_success = BasicConversion();
  if (!parse_success) parse_success = SpecialCaseConversion();
  if (!parse_success || uncovered_case) return;

  CheckForHTML();
  UnUtf();
  EscapeHTML();
}

void CharNumCorrection::ZeroString() {
  uncovered_case = false;
  needs_escape_sequence = false;
  log_out = false;
  char_correction = 0;
  for (size_t i = 0; i < CHAR_CONVERSION_BUFF_MAX_SIZE; i++) {
    correction[i] = '\0';
  }
}

void CharNumCorrection::UnUtf() {
  uint32_t byte_copy = char_correction;
  const size_t byte_arr_size = 4;
  uint32_t byte_arr[byte_arr_size];
  size_t byte_arr_len = 0;
  bool multi_byte = (byte_copy | 0x7f) > 0x7f;
  bool too_beeg = (byte_copy | 0x10FFFF) > 0x10FFFF;

  if (!multi_byte) {
    correction[0] = (char)byte_copy;
    return;
  }

  if (too_beeg) {
    std::cerr << "Bytes are too big for utf-8!!" << std::endl;
    return;
  }

  uint32_t first_byte_clear = 0xf0;
  uint32_t cont_byte_clear = 0x80;

  while (byte_copy) {
    uint32_t byte_correction = (byte_copy & 0x3f) | cont_byte_clear;
    byte_arr[byte_arr_len++] = byte_correction;
    byte_copy >>= 6;
  }

  first_byte_clear = (first_byte_clear << (byte_arr_size - byte_arr_len));
  byte_arr[byte_arr_len - 1] ^= cont_byte_clear;
  byte_arr[byte_arr_len - 1] |= first_byte_clear;

  for (size_t i = 0; byte_arr_len > 0 && i < byte_arr_size; byte_arr_len-- && i++) {
    correction[i] = (char)byte_arr[byte_arr_len - 1];
  }

}

void CharNumCorrection::CheckForHTML() {
  if (
    char_correction == '<' ||
    char_correction == '>' ||
    char_correction == '&' ||
    char_correction == '"' ||
    char_correction == '\'' ||
    char_correction == '`'
  ) {
    needs_escape_sequence = true;
  }
}

void CharNumCorrection::EscapeHTML() {
  if (!needs_escape_sequence) return;
  utf8_utils::sanitize_utf8_for_html(correction, CHAR_CONVERSION_BUFF_MAX_SIZE);
}
