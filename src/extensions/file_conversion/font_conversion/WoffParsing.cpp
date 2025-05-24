#include "WoffParsing.hpp"
#include "Endian.hpp"
#include "zconf.h"

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <zlib.h>

const bool DEBUG = false;

WOFFParser::WOFFParser(std::string file_name, std::string destination) : swapper(EndianOrder::Big) {
  file = std::ifstream(file_name, std::ios::binary);

  if (!CheckFileOpen()) {
    return;
  }

  file.read(reinterpret_cast<char *>(&signature), sizeof(signature));
  signature = swapper.Swap32(signature);
  ResetFilePos();

  switch (signature) {
    case (uint32_t)WoffSignature::wOFF:
      file.read(reinterpret_cast<char *>(&woff1.header), sizeof(woff1.header));
      ByteSwapHeader();
      woff1.table_directory = std::vector<WOFFTableDirectoryEntry>(woff1.header.num_tables);
      file.read(reinterpret_cast<char *>(woff1.table_directory.data()), sizeof(WOFFTableDirectoryEntry) * woff1.header.num_tables);
      is_good = true;
      if (DEBUG) std::cout << "wOFF File Detected" << std::endl;
      break;
    case (uint32_t)WoffSignature::wOF2:
      file.read(reinterpret_cast<char *>(&woff2.header), sizeof(woff2.header));
      if (DEBUG) std::cout << "wOF2 File Detected" << std::endl;
      std::cout << "wOF2 unsupported!" << std::endl;
      break;
    default:
      std::cout << "WOFF NOT DETECTED!!" << std::endl;
      break;
  }
}

WOFFParser::~WOFFParser() {
  if (file.is_open()) file.close();
}

void WOFFParser::ResetFilePos() {
  if (!CheckFileOpen("File Not Open! Unable to reset file.")) return;

  file.clear();
  file.seekg(0, std::ios::beg);
}

bool WOFFParser::CheckFileOpen(std::string msg) {
  bool is_open = file.is_open();

  if (!is_open) {
    std::cerr << msg << std::endl;
  }

  return is_open;
}

void WOFFParser::ByteSwapHeader() {
  woff1.header.signature = swapper.Swap32(woff1.header.signature);
  woff1.header.flavor = swapper.Swap32(woff1.header.flavor);
  woff1.header.length = swapper.Swap32(woff1.header.length);
  woff1.header.num_tables = swapper.Swap16(woff1.header.num_tables);
  woff1.header.reserved = swapper.Swap16(woff1.header.reserved);
  woff1.header.total_sfnt_size = swapper.Swap32(woff1.header.total_sfnt_size);
  woff1.header.major_version = swapper.Swap16(woff1.header.major_version);
  woff1.header.minor_version = swapper.Swap16(woff1.header.minor_version);
  woff1.header.meta_offset = swapper.Swap32(woff1.header.meta_offset);
  woff1.header.meta_length = swapper.Swap32(woff1.header.meta_length);
  woff1.header.meta_orig_length = swapper.Swap32(woff1.header.meta_orig_length);
  woff1.header.priv_off_set = swapper.Swap32(woff1.header.priv_off_set);
  woff1.header.priv_length = swapper.Swap32(woff1.header.priv_length);

}

void WOFFParser::ByteSwapTableDir(WOFFTableDirectoryEntry &table) {
  table.tag = swapper.Swap32(table.tag);
  table.offset = swapper.Swap32(table.offset);
  table.comp_length = swapper.Swap32(table.comp_length);
  table.orig_length = swapper.Swap32(table.orig_length);
  table.orig_checksum = swapper.Swap32(table.orig_checksum);
}


void WOFFParser::ReadTables() {
  /*
    std::vector<char> fontData(header.totalSfntSize);
    file.seekg(sizeof(WOFFHeader));
    file.read(fontData.data(), header.totalSfntSize);
  */
  for (WOFFTableDirectoryEntry& table : woff1.table_directory) {
    if (table.orig_length == 0) continue;
    ByteSwapTableDir(table);
    // std::cout << "--------------------" << std::endl;
    // std::cout << table.comp_length << std::endl;
    // std::cout << table.orig_length << std::endl;
    // std::cout << table.offset << std::endl;
    // std::cout << "--------------------" << std::endl;
    file.seekg(table.offset, std::ios::beg);
    std::vector<char> compressed_data(table.comp_length);
    file.read(compressed_data.data(), table.comp_length);

    if (table.comp_length != table.orig_length) {
      WFUncompress(table, compressed_data);
    } else {
      ProcessCompressed(table, compressed_data);
    }

  }
}

void WOFFParser::WFUncompress(const WOFFTableDirectoryEntry &table, std::vector<char> &compressed_data) {
  std::vector<char> uncompressed_data(table.orig_length);
  unsigned long uncompressed_size = table.orig_length;
  int result = uncompress((Bytef *)uncompressed_data.data(), &uncompressed_size, (const Bytef *)compressed_data.data(), table.comp_length);
  if (result == Z_OK) {
    ProcessUncompressed(table, uncompressed_data);
  } else {
    std::cerr << "Error in decompressing data, error code: " << result << std::endl;
    switch (result) {
      case Z_ERRNO:
        std::cout << "\tZ_ERRNO" << std::endl;
        break;
      case Z_STREAM_ERROR:
        std::cout << "\tStream Error" << std::endl;
        break;
      case Z_DATA_ERROR:
        std::cout << "\tData Error" << std::endl;
        break;
      case Z_MEM_ERROR:
        std::cout << "\tMemory Error" << std::endl;
        break;
      case Z_BUF_ERROR:
        std::cout << "\tBuffer Error" << std::endl;
        break;
      case Z_VERSION_ERROR:
        std::cout << "\tVersion Error" << std::endl;
        break;
      default:
        std::cout << "Unspecified Error" << std::endl;
        break;
    }
  }
}

void WOFFParser::ProcessUncompressed(const WOFFTableDirectoryEntry &table, std::vector<char> &uncompressed_data) {
  // process
  // std::cout << uncompressed_data.data() << std::endl;
  std::string str(uncompressed_data.begin(), uncompressed_data.end());
  std::cout << str << std::endl;
}

void WOFFParser::ProcessCompressed(const WOFFTableDirectoryEntry &table, std::vector<char> &compressed_data) {
  // process
  // std::cout << compressed_data.data() << std::endl;
  std::string str(compressed_data.begin(), compressed_data.end());
  std::cout << str << std::endl;
}
