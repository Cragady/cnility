#ifndef EXTENSIONS_FILE_CONVERSION_CHAR_NUM_CORRECTION_HPP_
#define EXTENSIONS_FILE_CONVERSION_CHAR_NUM_CORRECTION_HPP_

// NOTE: we set this to 5 for the NULL byte
#define CHAR_CONVERSION_BUFF_SIZE 5
#define CHAR_CONVERSION_BUFF_MAX_SIZE 16

#include <cstdint>

typedef long long ll;

class CharNumCorrection {
private:
  ll _char_distance = 58849;
  ll _chars_start = 0xe602;
  ll _char_specials = 0xe669;
  ll _char_special_capitals_distance = 59094;

public:
  uint32_t char_to_convert = 0;
  uint32_t char_correction = 0;
  bool parse_success = false;
  bool uncovered_case = false;
  bool needs_escape_sequence = false;
  bool log_out = false;
  char correction[CHAR_CONVERSION_BUFF_MAX_SIZE];

  bool BasicConversion();
  bool SpecialCaseConversion(bool log_case = false);
  void Conversion();
  void ZeroString();
  void UnUtf();
  void CheckForHTML();
  void EscapeHTML();
};

#endif
