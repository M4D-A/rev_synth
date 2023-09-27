#include "state.hpp"
#include "iostream"
#include <cassert>
#include <cstdint>
#include <limits>

const auto mask64 = std::numeric_limits<uint64_t>::max();

state::state(uint64_t bits_num, uint64_t value)
    : bits_num_(bits_num), bits_mask_(mask64 >> (64 - bits_num)) {
  assert(bits_num_ >= 1 && bits_num_ <= 64);
  set_value(value);
}

auto state::bits_num() const -> uint64_t { return bits_num_; }

auto state::bits_mask() const -> uint64_t { return bits_mask_; }

auto state::value() const -> uint64_t { return value_; }

auto state::bit_value(uint64_t index) const -> uint64_t {
  return (value_ >> index) & 1UL;
}

auto state::zeroes() const -> std::vector<uint64_t> {
  auto retval = std::vector<uint64_t>();
  for (auto i = 0UL; i < bits_num_; i++) {
    if (bit_value(i) == 0) {
      retval.push_back(i);
    }
  }
  return retval;
}

auto state::ones() const -> std::vector<uint64_t> {
  auto retval = std::vector<uint64_t>();
  for (auto i = 0UL; i < bits_num_; i++) {
    if (bit_value(i) == 1) {
      retval.push_back(i);
    }
  }
  return retval;
}

auto state::set_value(uint64_t value) -> state & {
  value_ = value & bits_mask_;
  return *this;
}

auto state::set_bit(uint64_t index, uint64_t bit_value) -> state & {
  assert(bit_value == 0UL || bit_value == 1UL);
  assert(index < bits_num_);
  auto rhs = bit_value << index;
  auto mask = ~(1UL << index);
  auto lhs = value_ & mask;
  value_ = lhs | rhs;
  return *this;
}

auto state::switch_bit(uint64_t index) -> state & {
  assert(index < bits_num_);
  auto mask = 1UL << index;
  value_ ^= mask;
  return *this;
}

auto state::negate() -> state & {
  value_ = (~value_) & bits_mask_;
  return *this;
}

auto state::operator==(const uint64_t &rhs) const -> bool {
  return rhs == value_;
}

auto state::operator[](const uint64_t index) const -> uint64_t {
  return bit_value(index);
}

state::operator uint64_t() const { return value(); }

auto state::print() const -> void {
  for (auto i = bits_num_; i > 0UL; i--) {
    auto bit = (value_ >> (i - 1UL)) & 1UL;
    std::cout << bit;
  }
  std::cout << std::endl;
}
