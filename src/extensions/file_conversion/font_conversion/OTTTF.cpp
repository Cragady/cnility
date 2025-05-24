#include "OTTTF.hpp"
#include "Endian.hpp"

OTTTF::OTTTF() : endian(EndianOrder::Big) {}

void OTTTF::SwapHeaders() {
  base_header.snft_or_scaler = endian.Swap32(base_header.snft_or_scaler);
  base_header.num_tables = endian.Swap16(base_header.num_tables);
  base_header.search_range = endian.Swap16(base_header.search_range);
  base_header.entry_selector = endian.Swap16(base_header.entry_selector);
  base_header.range_shift = endian.Swap16(base_header.range_shift);
}
