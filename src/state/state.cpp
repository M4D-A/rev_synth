#include "state.hpp"
#include "iostream"
#include <cassert>
#include <limits>

const uint64_t mask64 = std::numeric_limits<uint64_t>::max();

state::state(uint64_t bits_num_, uint64_t value_)
    : bits_num(bits_num_), bits_mask(mask64 >> (64 - bits_num)) {
  assert(bits_num_ >= 1 && bits_num_ <= 64);
  set_value(value_);
}

auto state::get_bits_num() const -> uint64_t { return bits_num; }

auto state::get_bits_mask() const -> uint64_t { return bits_mask; }

auto state::get_value() const -> uint64_t { return value; }

auto state::get_bit(uint64_t index) const -> uint64_t {
  return (value >> index) & 1UL;
}

void state::set_value(uint64_t new_value) { value = new_value & bits_mask; }

void state::set_bit(uint64_t index, uint64_t bit_value) {
  assert(bit_value == 0UL || bit_value == 1UL);
  assert(index < bits_num);
  auto rhs = bit_value << index;
  auto mask = ~(1UL << index);
  auto lhs = value & mask;
  value = lhs | rhs;
}

void state::switch_bit(uint64_t index) {
  assert(index < bits_num);
  auto mask = 1UL << index;
  value ^= mask;
}

void state::negate() { value = (~value) & bits_mask; }

void state::print() const {
  for (auto i = bits_num; i > 0UL; i--) {
    auto bit = (value >> (i - 1UL)) & 1UL;
    std::cout << bit;
  }
  std::cout << std::endl;
}
