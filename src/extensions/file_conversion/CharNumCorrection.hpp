#ifndef EXTENSIONS_FILE_CONVERSION_CHAR_NUM_CORRECTION_HPP_
#define EXTENSIONS_FILE_CONVERSION_CHAR_NUM_CORRECTION_HPP_

// NOTE: we set this to 5 for the NULL byte
#define CHAR_CONVERSION_BUFF_SIZE 5

#include <cstdint>

typedef long long ll;

class CharNumCorrection {
private:
  ll _char_distance = 58849;
  ll _char_specials = 0xe669;

public:
  uint32_t char_to_convert = 0;
  uint32_t char_correction = 0;
  bool parse_success = false;
  char correction[CHAR_CONVERSION_BUFF_SIZE];

  bool BasicConversion();
  bool SpecialCaseConversion();
  void Conversion();
  void ZeroString();
  void UnUtf();
};

#endif
