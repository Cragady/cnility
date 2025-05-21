#ifndef EXTENSIONS_FILE_CONVERSION_FILE_CONVERSION_HPP_
#define EXTENSIONS_FILE_CONVERSION_FILE_CONVERSION_HPP_

#include "CharNumCorrection.hpp"
#include <cstdint>
#include <fstream>
#include <string>

#define BUFF_MAX 4

typedef unsigned long long ull;
typedef long long ll;
typedef unsigned char uc;

enum class UTFByteType {
  One,
  Two,
  Three,
  Four,
};

class FileConversion {
private:
  std::string _file_name;
  std::string _destination;
  std::string _converted_file_data;
  std::ifstream _file_data;
  size_t _buff_size = 0;
  char _char_buff[BUFF_MAX];
  size_t _chars_parsed = 0;
  size_t _buff_pos = 0;
  bool _log_char_stream = false;

  CharNumCorrection _char_corrector;

  UTFByteType _byte_type = UTFByteType::One;
public:
  bool write_file = false;

  FileConversion(std::string file_name, std::string destination);

  void LogValues();
  void LoopFile();
  bool BuffNumber(char num);
  bool ParseOutNum();
  bool CheckLeadingUTF8Char(char c);
  bool CheckContinuationByte(char c);
  void ResetByteValues();
  void ResetCharBuff();
  void SetBuffSize(size_t size);
  void LogByteInfo();
  void LogCharacterInfo();
  void LogRegularCharInWidthOfInfo(char c);
  void LogCharStream(char c = (char)NULL);
  void ConvertCharStream(char c = (char)NULL);
  void WriteToFile();
};

#endif
