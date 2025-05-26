#ifndef EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_TABLES_CMAP_HPP_
#define EXTENSIONS_FILE_CONVERSION_FONT_CONVERSION_TABLES_CMAP_HPP_
// ####################
// ##  TTF SUBTABLE  ##
// ####################

#include <cstdint>

struct CmapIndex {
  uint16_t version; // Version number (Set to zero)
  uint16_t number_sub_tables; // Number of encoding subtables
};

// NOTE: The 'cmap' subtables must be sorted first in ascending order by
// platform identifier and then by platform-specific identifier.

struct CmapEncodingSubtable {
  uint16_t platform_id; // Platform identifier
  uint16_t platform_specific_id; // Platform-specific encoding identifier
  uint32_t offset; // Offset of the mapping table
};

// NOTE: The 'cmap' subtables must be sorted first in ascending order by
// platform identifier and then by platform-specific identifier.

enum class CmapPlatforms {
  Unicode,
  Macintosh,
  RESERVED_DO_NOT_USE,
  Microsoft,
};

enum class PlatformSpecificIdCode_Unicode {
  Version_1_0_Semantics,
  Version_1_1_Semantics,
  ISO_10646_1993_Semantics_Deprecated,
  Unicode_2_0_Or_Later_Semantics_BMP_Only,
  Unicode_2_0_Or_Later_Semantics_Non_BMP_Characters_Allowed,
  Unicode_Variation_Sequences,
  Last_Resort,
};

enum class PlatformSpecificIdCode_Macintosh {
  CURRENTY_DISCOURAGED_IF_IDS_NEEDED_VISIT_URL_IN_COMMENT_BELOW = 9999,
  // https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6name.html
};

enum class PlatformSpecificIdCode_RESERVED_DO_NOT_USE {
  RESERVED_DO_NOT_USE,
};

enum class PlatformSpecificIdCode_Windows {
  Symbol,
  Unicode_BMP_Only_UCS_2,
  Shift_JIS,
  PRC,
  Big_Five,
  Johab,
  Unicode_UCS_4,
};

struct CmapFormat {
  uint16_t format;
};

struct CmapFormat_0 {
  uint16_t length;
  uint16_t language;
  uint8_t glyph_index_array[256];
};

// <type-specifier> PlatformSpecificIdCode {
//   PlatformSpecificIdCode_Unicode unicode;
//   PlatformSpecificIdCode_Macintosh macintosh;
//   PlatformSpecificIdCode_RESERVED_DO_NOT_USE RESERVED_DO_NOT_USE;
//   PlatformSpecificIdCode_Windows windows;
// };

#endif
