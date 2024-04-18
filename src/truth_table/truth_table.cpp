#include "truth_table.hpp"
#include "state/state.hpp"
#include <cassert>
#include <numeric>
#include <random>
#include <stdexcept>
#include <sys/types.h>

truth_table::truth_table(uint64_t bits_num)
    : _size(state::check_size(bits_num)), _mask(state::mask(bits_num)) {
  _data = std::vector<uint64_t>(1UL << bits_num);
  std::iota(_data.begin(), _data.end(), 0);
}

auto truth_table::size() const noexcept -> uint64_t { return _size; }

auto truth_table::length() const noexcept -> uint64_t { return _data.size(); }

auto truth_table::mask() const noexcept -> uint64_t { return _mask; }

auto truth_table::data() const noexcept -> const std::vector<uint64_t> & { return _data; }

auto truth_table::row(uint64_t index) const -> uint64_t {
  if (index > this->length()) {
    throw std::invalid_argument("Rows index has to be smaller than length of truth_table");
  }
  return _data[index];
}

auto truth_table::state(uint64_t index) const -> class state {
  auto row = this->row(index);
  return {size(), row};
}

auto truth_table::set_data(std::vector<uint64_t> new_data) -> truth_table & {
  auto length = this->length();
  if (new_data.size() != length) {
    throw std::invalid_argument("Length of asignee data is invalid");
  }
  if (std::any_of(new_data.begin(), new_data.end(), [this](auto e) { return e > this->_mask; })) {
    throw std::invalid_argument(
        "Asignee data row elements have to be shorter than size of truth table");
  }
  _data = std::move(new_data);
  return *this;
}

auto truth_table::set_row(uint64_t index, uint64_t value) -> truth_table & {
  if (value > _mask) {
    throw std::invalid_argument("Row has to be shorter than size of truth table");
  }
  _data[index] = value;
  return *this;
}

auto truth_table::shuffle(std::mt19937_64 &mrnd) -> truth_table & {
  std::shuffle(_data.begin(), _data.end(), mrnd);
  return *this;
}

auto truth_table::swap(uint64_t index_1, uint64_t index_2) noexcept(false) -> truth_table & {
  if (index_1 > this->length() || index_2 > this->length()) {
    throw std::invalid_argument(
        "Swapped rows indicies have to be smaller than length of truth_table");
  }
  std::swap(_data[index_1], _data[index_2]);
  return *this;
}

auto truth_table::inverse() -> truth_table & {
  auto new_data = std::vector<uint64_t>(length());
  for (auto input = 0UL; input < length(); input++) {
    auto output = _data[input];
    new_data[output] = input;
  }
  set_data(new_data);
  return *this;
}

auto truth_table::next_permutation() -> bool {
  return std::next_permutation(_data.begin(), _data.end());
}

auto truth_table::begin() -> std::vector<uint64_t>::iterator { return _data.begin(); }

auto truth_table::end() -> std::vector<uint64_t>::iterator { return _data.end(); }

auto truth_table::operator[](uint64_t index) -> uint64_t & { return _data[index]; }

auto truth_table::operator[](uint64_t index) const -> uint64_t { return _data[index]; }

auto truth_table::operator+(const truth_table &rhs) const -> truth_table {
  assert(length() == rhs.length());
  assert(size() == rhs.size());
  auto result = *this;

  for (auto input = 0UL; input < length(); input++) {
    auto output = rhs[(*this)[input]];
    result.set_row(input, output);
  }

  return result;
}

auto truth_table::operator+=(const truth_table &rhs) -> truth_table & {
  assert(length() == rhs.length());
  assert(size() == rhs.size());
  for (auto input = 0UL; input < length(); input++) {
    auto output = rhs[(*this)[input]];
    set_row(input, output);
  }
  return *this;
}

auto truth_table::print() const -> void {
  for (auto i = 0UL; i < length(); i++) {
    state(i).print();
  }
}
