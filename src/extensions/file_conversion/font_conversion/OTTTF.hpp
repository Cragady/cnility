#ifndef EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_OTTTF_HPP_
#define EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_OTTTF_HPP_

#include "Endian.hpp"

#include <cstdint>

struct BaseOpenHeader {
  uint32_t snft_or_scaler;
  uint16_t num_tables;
  uint16_t search_range;
  uint16_t entry_selector;
  uint16_t range_shift;
};

class OTTTF {
private:
  Endian endian;
public:
  BaseOpenHeader base_header;

  OTTTF();
  void SwapHeaders();
};

#endif
