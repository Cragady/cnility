#ifndef EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_ENDIAN_HPP_
#define EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_ENDIAN_HPP_

#include <cstddef>
#include <cstdint>

enum class EndianOrder {
  Big,
  Little,
};

class Endian {
private:
public:
  EndianOrder host;
  EndianOrder target;
  uint8_t filter_byte = 0xff;
  size_t byte_width = 8;
  size_t num_bytes = 4;

  Endian(EndianOrder target_arg);

  uint16_t Swap16(uint16_t bytes);
  uint32_t Swap32(uint32_t bytes);
  uint64_t Swap64(uint64_t bytes);

  uint64_t Swap(uint64_t bytes);
};

#endif
