#ifndef EXTENSIONS_CHAR_CONVERSION_CHAR_CONVERSION_HPP_
#define EXTENSIONS_CHAR_CONVERSION_CHAR_CONVERSION_HPP_

#include <fstream>
#include <string>

typedef unsigned long long ull;
typedef long long ll;

class FileConversion {
private:
  std::string _file_name;
  std::string _destination;
  std::ifstream _file_data;
  // NOTE: following incorrect
  // ull _char_distance = 18446744073709551598 - (ull)'P';
  ll _char_distance = -159;
  ull _char_portion_distance = 18446744073709551000ull;
  ull _max_char_distance;
  size_t _buff_size = 3;
  ll _char_buff[3];
  size_t _buff_pos = 0;
public:
  bool write_file = false;

  FileConversion(std::string file_name, std::string destination, ull char_distance, ull max_char_distance);

  void LogValues();
  void LoopFile();
  bool BuffNumber(ll num);
  void ParseThree();
};

#endif
