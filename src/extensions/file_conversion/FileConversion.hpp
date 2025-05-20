#ifndef EXTENSIONS_CHAR_CONVERSION_CHAR_CONVERSION_HPP_
#define EXTENSIONS_CHAR_CONVERSION_CHAR_CONVERSION_HPP_

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
  std::ifstream _file_data;
  ll _char_distance = 58849;
  size_t _buff_size = 0;
  char _char_buff[BUFF_MAX];
  char _converted_char;
  size_t _buff_pos = 0;

  UTFByteType _byte_type = UTFByteType::One;
public:
  bool write_file = false;

  FileConversion(std::string file_name, std::string destination);

  void LogValues();
  void LoopFile();
  bool BuffNumber(char num);
  bool ParseOutNum();
  uint32_t ParseMultiByte();
  bool CheckLeadingUTF8Char(char c);
  bool CheckContinuationByte(char c);
  void ResetByteValues();
  void ResetCharBuff();
  void SetBuffSize(size_t size);
  void LogByteInfo();
};

#endif
