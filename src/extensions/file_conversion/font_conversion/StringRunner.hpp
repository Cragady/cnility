#ifndef EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_STRING_RUNNER_HPP_
#define EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_STRING_RUNNER_HPP_

#include "Endian.hpp"
#include <cstddef>
#include <string>

class StringRunner {
private:
  std::string str_;
  size_t str_pos_ = 0;
  Endian endian_;
  size_t RevReadStr_(char *dest, size_t size);
public:
  // TODO: get rid of magic
  char tag[5];

  StringRunner(std::string);

  void LogStr();
  void LogStrFromInternalPos();
  size_t ReadStr(char *dest, size_t size, bool allow_read_rev = false);
  void ReadStrStatic(char *dest, size_t size);
  void PlacePos(size_t pos);
  void SetTag(uint32_t tag_nums);
  void LogStringBoundsReadError(const char *msg = nullptr);
};

#endif
