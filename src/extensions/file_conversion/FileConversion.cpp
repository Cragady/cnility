#include <iostream>
#include <fstream>
#include <string>

#include "FileConversion.hpp"

FileConversion::FileConversion(std::string file_name, std::string destination, ull char_distance, ull max_char_distance) {
  _file_name = file_name;
  _file_data = std::ifstream(file_name);
  _destination = destination;
  // _char_distance = char_distance;
  _max_char_distance = max_char_distance;
}

void FileConversion::LogValues() {
  std::cout << "_file_name: " << _file_name << std::endl;
  std::cout << "_destination: " << _destination << std::endl;
  std::cout << "_char_distance: " << _char_distance << std::endl;
  std::cout << "_max_char_distance: " << _max_char_distance << std::endl;
}

void FileConversion::LoopFile() {
  if (_file_data.is_open()) {
    char c;
    while (_file_data.get(c)) {
      if ((ull)c > _max_char_distance) {
        std::cout << (ll)(unsigned char)c << " ";
        bool buff_added = BuffNumber(c);
        if (buff_added && _buff_pos <= 2) {
          continue;
        }
        _buff_pos = 0;
        ParseThree();
        // BuffNumber(c);
        continue;
      }
    }
    std::cout << "\n" << std::endl;
    _file_data.close();
    return;
  }

  std::cerr << "Unable to open file" << std::endl;
}

bool FileConversion::BuffNumber(char num) {
  if (_buff_pos > _buff_size - 1) return false;
  _char_buff[_buff_pos] = num;
  _buff_pos++;
  return true;
}

void FileConversion::ParseThree() {
  uint32_t target_num = ParseThreeByte8(_char_buff);

  // TODO: correct the below to include a larget amount of the glyph table
  // Also of note: there are custom glyphs on this font that don't *appear*
  // to be in the unicode table
  target_num -= _char_distance;

  std::cout << (char)target_num << std::endl;;

}

uint32_t FileConversion::ParseThreeByte8(const char *str) {
  unsigned char *bytes = (unsigned char *)str;


  // NOTE: 3-byte UTF-8 parsing. We don't need to cover other byte cases since
  // the font table doesn't really go lower than 3 bytes
  if ((bytes[0] & 0xf0) == 0xe0) {
    return ((bytes[0] & 0x0f) << 12) |
      ((bytes[1] & 0x3f) << 6) |
      (bytes[2] & 0x3F);
  }
  return 0;
}
