#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

#include "FileConversion.hpp"

FileConversion::FileConversion(std::string file_name, std::string destination) {
  _file_name = file_name;
  _file_data = std::ifstream(file_name);
  _destination = destination;
}

void FileConversion::LogValues() {
  std::cout << "_file_name: " << _file_name << std::endl;
  std::cout << "_destination: " << _destination << std::endl;
  std::cout << "_char_distance: " << _char_distance << std::endl;
}

void FileConversion::LoopFile() {
  if (_file_data.is_open()) {
    char c;
    while (_file_data.get(c)) {
      bool is_multi_byte = CheckLeadingUTF8Char(c);
      if (is_multi_byte) {
        bool buff_added = BuffNumber(c);
        if (!buff_added) continue;
        bool parsed = ParseOutNum();
        if (parsed) ResetByteValues();
      }
    }
    std::cout << "\n" << std::endl;
    _file_data.close();
    return;
  }

  std::cerr << "Unable to open file" << std::endl;
}

bool FileConversion::BuffNumber(char num) {
  bool valid_cont_byte = CheckContinuationByte(num);
  if (!valid_cont_byte && _buff_pos != 0) {
    ResetByteValues();
    return false;
  }
  if (_buff_pos > _buff_size - 1) return false;
  _char_buff[_buff_pos] = num;
  _buff_pos++;
  return true;
}

bool FileConversion::ParseOutNum() {
  // NOTE: _buff_pos will be incremented to the _buff_size on the last possible
  // successful addition, so we do a direct comparison
  if (_buff_pos != _buff_size) return false;
  uint32_t target_num = ParseMultiByte();

  // TODO: correct the below to include a larget amount of the glyph table
  // Also of note: there are custom glyphs on this font that don't *appear*
  // to be in the unicode table
  target_num -= _char_distance;
  _converted_char = (char)target_num;

  LogByteInfo();

  return true;
}

bool FileConversion::CheckLeadingUTF8Char(char c) {
  // NOTE: This is a simple parser, I am not checking if the bytes contain
  // unused bytes in the UTF-8 byte map
  if ((c & 0xe0) == 0xc0) {
    _byte_type = UTFByteType::Two;
    SetBuffSize(2);
  }
  if ((c & 0xf0) == 0xe0) {
    _byte_type = UTFByteType::Three;
    SetBuffSize(3);
  }
  // NOTE: 0xf5 - 0xff are unused in the UTF-8 byte map
  if ((c & 0xff) >= 0xf0) {
    _byte_type = UTFByteType::Four;
    SetBuffSize(4);
  }

  return _byte_type != UTFByteType::One;
}

bool FileConversion::CheckContinuationByte(char c) {
  return (c & 0xc0) == 0x80;
}

uint32_t FileConversion::ParseMultiByte() {
  unsigned char *bytes = (unsigned char *)_char_buff;

  uint32_t num = 0;
  for (size_t i = 0; i < _buff_size; i++) {
    int clear_bit = 0x3f;
    int offset = (_buff_size - 1) * 6 - i * 6;
    if (i == 0) {
      clear_bit = 0x0f;
    }
    num |= ((bytes[i] & clear_bit) << offset);
  }

  return num;
}

void FileConversion::ResetByteValues() {
  _byte_type = UTFByteType::One;
  ResetCharBuff();
}

void FileConversion::ResetCharBuff() {
  _buff_size = 0;
  _buff_pos = 0;
  for (size_t i = 0; i < BUFF_MAX; i++) {
    _char_buff[i] = '\0';
  }
}

void FileConversion::SetBuffSize(size_t size) {
  ResetCharBuff();
  _buff_size = size;
}

void FileConversion::LogByteInfo() {
  std::cout << std::left;
  for (size_t i = 0; i < BUFF_MAX; i++) {
    char c = _char_buff[i];
    if (c) std::cout << std::setw(5) << (int)_char_buff[i];
  }
  std::cout << " | " << _converted_char << std::endl;
}
