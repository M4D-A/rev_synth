#include "truth_table.hpp"
#include <cassert>
#include <numeric>
#include <sys/types.h>

const uint64_t mask64 = std::numeric_limits<uint64_t>::max();

truth_table::truth_table(uint64_t bits_num_)
    : bits_num(bits_num_), bits_mask(mask64 >> (64 - bits_num)) {
  assert(bits_num_ >= 1 && bits_num_ <= 64);
  data = std::vector<uint64_t>(1 << bits_num);
  std::iota(data.begin(), data.end(), 0);
}

auto truth_table::get_bits_mask() const -> uint64_t { return bits_mask; }

auto truth_table::get_bits_num() const -> uint64_t { return bits_num; }

auto truth_table::get_data() const -> std::vector<uint64_t> { return data; }

auto truth_table::get_row(uint64_t index) const -> uint64_t {
  return data[index];
}

auto truth_table::set_data(std::vector<uint64_t> new_data) -> void {
  data = new_data;
}

auto truth_table::set_row(uint64_t index, uint64_t value) -> void {
  data[index] = value;
}
