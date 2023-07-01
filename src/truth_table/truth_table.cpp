#include "truth_table.hpp"
#include <cassert>
#include <numeric>
#include <sys/types.h>

const uint64_t mask64 = std::numeric_limits<uint64_t>::max();

truth_table::truth_table(uint64_t bits_num)
    : bits_num_(bits_num), bits_mask_(mask64 >> (64 - bits_num)) {
  assert(bits_num >= 1 && bits_num <= 64);
  data_ = std::vector<uint64_t>(1 << bits_num);
  std::iota(data_.begin(), data_.end(), 0);
}

auto truth_table::get_bits_num() const -> uint64_t { return bits_num_; }

auto truth_table::get_size() const -> uint64_t { return data_.size(); }

auto truth_table::get_bits_mask() const -> uint64_t { return bits_mask_; }

auto truth_table::get_data() const -> std::vector<uint64_t> { return data_; }

auto truth_table::get_row(uint64_t index) const -> uint64_t {
  return data_[index];
}

auto truth_table::set_data(std::vector<uint64_t> new_data) -> void {
  data_ = new_data;
}

auto truth_table::set_row(uint64_t index, uint64_t value) -> void {
  data_[index] = value;
}
