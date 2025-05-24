#ifndef EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_WOFF_PARSING_HPP_
#define EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_WOFF_PARSING_HPP_

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include "Endian.hpp"

enum class WoffSignature : uint32_t {
  wOFF = 0x774F4646,
  wOF2 = 0x774F4632,
};

// NOTE: no inheritance to keep EXACT control over data placement and alignment
// NOTE: leaving off signature to avoid having to 'reset' files

struct WOFFHeader {
  uint32_t signature; // 0x774F4646 'wOFF'
  uint32_t flavor; // The "sfnt version" of the input font.
  uint32_t length; // Total size of the WOFF file.
  uint16_t num_tables; // Number of entries in directory of font tables.
  uint16_t reserved; // Reserved; set to zero.
  uint32_t total_sfnt_size; // Total size needed for the uncompressed font data, including the sfnt header, directory, and font tables (including padding).
  uint16_t major_version; //	Major version of the WOFF file.
  uint16_t minor_version; //	Minor version of the WOFF file.
  uint32_t meta_offset; //	Offset to metadata block, from beginning of WOFF file.
  uint32_t meta_length; //	Length of compressed metadata block.
  uint32_t meta_orig_length; //	Uncompressed size of metadata block.
  uint32_t priv_off_set; //	Offset to private data block, from beginning of WOFF file.
  uint32_t priv_length; //	Length of private data block.
};

// NOTE: no inheritance to keep EXACT control over data placement and alignment
// NOTE: leaving off signature to avoid having to 'reset' files

struct WOFF2Header {
  uint32_t signature; // 0x774F4632 'wOF2'
  uint32_t flavor; // The "sfnt version" of the input font.
  uint32_t length; // Total size of the WOFF file.
  uint16_t num_tables; // Number of entries in directory of font tables.
  uint16_t reserved; // Reserved; set to zero.
  uint32_t total_sfnt_size; // Total size needed for the uncompressed font data, including the sfnt header, directory, and font tables (including padding).
  uint32_t total_compressed_size; // Total length of the compressed data block.
  uint16_t major_version; //	Major version of the WOFF file.
  uint16_t minor_version; //	Minor version of the WOFF file.
  uint32_t meta_offset; //	Offset to metadata block, from beginning of WOFF file.
  uint32_t meta_length; //	Length of compressed metadata block.
  uint32_t meta_orig_length; //	Uncompressed size of metadata block.
  uint32_t priv_off_set; //	Offset to private data block, from beginning of WOFF file.
  uint32_t priv_length; //	Length of private data block.
};

struct WOFFTableDirectoryEntry {
  uint32_t tag; // 4-byte sfnt table identifier.
  uint32_t offset; // Offset to the data, from beginning of WOFF file.
  uint32_t comp_length; // Length of the compressed data, excluding padding.
  uint32_t orig_length; // Length of the uncompressed table, excluding padding.
  uint32_t orig_checksum; // Checksum of the uncompressed table.
};

struct WOFF2TableDirectoryEntry {
  uint8_t	flags; // table type and flags
  uint32_t tag; // 4-byte tag (optional)
  uint32_t orig_length; // uintbase128 - length of original table
  uint32_t transform_length; // uintbase128 - transformed length (if applicable)
};


class WOFF1 {
public:
  WOFFHeader header;
  std::vector<WOFFTableDirectoryEntry> table_directory;
};

class WOFF2 {
public:
  WOFF2Header header;
};




class WOFFParser {
private:
public:
  Endian swapper;
  uint32_t signature; // WOFF Header Signature
  // char signature[4]; // WOFF Header Signature
  WOFF1 woff1;
  WOFF2 woff2;
  std::ifstream file;
  bool is_good = false;

  WOFFParser(std::string file_name, std::string destination);
  ~WOFFParser();

  void ResetFilePos();
  bool CheckFileOpen(std::string msg = "Error opening WOFF File.");
  void ByteSwapHeader();
  void ByteSwapTableDir(WOFFTableDirectoryEntry &table);
  void ReadTables();
  void WFUncompress(const WOFFTableDirectoryEntry &table, std::vector<char> &compressed_data);
  void ProcessUncompressed(const WOFFTableDirectoryEntry &table, std::vector<char> &uncompressed_data);
  void ProcessCompressed(const WOFFTableDirectoryEntry &table, std::vector<char> &compressed_data);

};


#endif
