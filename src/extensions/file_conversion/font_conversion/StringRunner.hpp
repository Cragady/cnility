#ifndef EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_STRING_RUNNER_HPP_
#define EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_STRING_RUNNER_HPP_

#include <cstddef>

class StringRunner {
private:
  const char *str_master_;
  char *str_;
public:
  StringRunner(const char *);
  void LogStr();
  void ReadStr(char *dest, size_t size);
};

#endif
