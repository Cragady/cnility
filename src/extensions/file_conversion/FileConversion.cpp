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
        // std::cout << (long long)c << " ";
        bool buff_added = BuffNumber((ll)c);
        if (buff_added) {
          continue;
        }
        _buff_pos = 0;
        ParseThree();
        BuffNumber((ll)c);
        continue;
        // three big ass numbers per char
        // 18446744073709551598
        // 18446744073709551512
        // 18446744073709551537
        // std::cout << (ull)c << ": ";
        // c = (ull)c - _char_distance;
        // std::cout << (ull)c << " ";
        // 18446744073709551598
        // break;
      }
      // std::cout << c;
    }
    // std::cout << "\n" << std::endl;
    _file_data.close();
    return;
  }

  std::cerr << "Unable to open file" << std::endl;
}

bool FileConversion::BuffNumber(ll num) {
  if (_buff_pos > _buff_size - 1) return false;
  _char_buff[_buff_pos] = num;
  _buff_pos++;
  return true;
}

void FileConversion::ParseThree() {
  char c;
  long long num = _char_buff[2];
  // for (size_t i = 0; i < _buff_size; i++) {
  //   num += _char_buff[i] - _char_portion_distance;
  // }

  ll pos1 = _char_buff[0];
  ll pos2 = _char_buff[1];
  ll pos3 = _char_buff[2];
  bool ok = true;

  if (pos1 == -18) {

    if (pos2 != -104 && pos2 != -103) {
      ok = false;
      std::cout << "Non standard second index num" << std::endl;
    }
  } else {
    ok = false;
    std::cout << "\n\nNON -18 NUM IN FIRST POS" << std::endl;
  }
  if (!ok) {
    std::cout << _file_name << std::endl;
    std::cout << _char_buff[0] << " ";
    std::cout << _char_buff[1] << " ";
    std::cout << _char_buff[2] << std::endl;
  }

  // std::cout << _char_buff[0] << " ";
  // std::cout << _char_buff[1] << " ";
  // std::cout << _char_buff[2];
  // std::cout << std::endl;
  // The below currently prints PREFACE with setup
  // num -= (long long)_char_distance;
  // std::cout << (char)num << " ";

}
