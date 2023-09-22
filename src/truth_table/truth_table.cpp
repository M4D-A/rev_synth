#include "truth_table.hpp"
#include <cassert>
#include <numeric>
#include <random>
#include <sys/types.h>

const uint64_t mask64 = std::numeric_limits<uint64_t>::max();

truth_table::truth_table(uint64_t bits_num)
    : bits_num_(bits_num), bits_mask_(mask64 >> (64 - bits_num)) {
  assert(bits_num >= 1 && bits_num <= 64);
  data_ = std::vector<uint64_t>(1 << bits_num);
  std::iota(data_.begin(), data_.end(), 0);
}

auto truth_table::bits_num() const -> uint64_t { return bits_num_; }

auto truth_table::size() const -> uint64_t { return data_.size(); }

auto truth_table::bits_mask() const -> uint64_t { return bits_mask_; }

auto truth_table::data() const -> std::vector<uint64_t> { return data_; }

auto truth_table::row(uint64_t index) const -> uint64_t { return data_[index]; }

auto truth_table::set_data(std::vector<uint64_t> new_data) -> truth_table & {
  data_ = std::move(new_data);
  return *this;
}

auto truth_table::set_row(uint64_t index, uint64_t value) -> truth_table & {
  data_[index] = value;
  return *this;
}

auto truth_table::shuffle(std::mt19937_64 &mrnd) -> truth_table & {
  std::shuffle(data_.begin(), data_.end(), mrnd);
  return *this;
}

auto truth_table::swap(uint64_t index_1, uint64_t index_2) -> truth_table & {
  std::swap(data_[index_1], data_[index_2]);
  return *this;
}

auto truth_table::inverse() -> truth_table & {
  auto new_data = std::vector<uint64_t>(size());
  for (auto input = 0UL; input < size(); input++) {
    auto output = data_[input];
    new_data[output] = input;
  }
  set_data(new_data);
  return *this;
}

auto truth_table::operator[](uint64_t index) -> uint64_t & {
  return data_[index];
}

auto truth_table::operator[](uint64_t index) const -> uint64_t {
  return data_[index];
}

auto truth_table::operator+(const truth_table &rhs) const -> truth_table {
  assert(size() == rhs.size());
  assert(bits_num() == rhs.bits_num());
  auto result = *this;

  for (auto input = 0UL; input < size(); input++) {
    auto output = rhs[(*this)[input]];
    result.set_row(input, output);
  }

  return result;
}

auto truth_table::operator+=(const truth_table &rhs) -> truth_table & {
  assert(size() == rhs.size());
  assert(bits_num() == rhs.bits_num());
  for (auto input = 0UL; input < size(); input++) {
    auto output = rhs[(*this)[input]];
    set_row(input, output);
  }
  return *this;
}

auto truth_table::print() const -> void {
  for (auto i = 0UL; i < size(); i++) {
    state(bits_num(), (*this)[i]).print();
  }
}
