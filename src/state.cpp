#include "state.hpp"
#include "iostream"
#include <cassert>

state::state(uint64_t bits_, uint64_t value_) {
  assert(bits >= 1 && bits <= 64);
  bits = bits_;
  set(value_);
}

auto state::get_bits_num() const -> uint64_t { return bits; }

auto state::get() const -> uint64_t { return value; }

auto state::get_bit(uint64_t index) const -> uint64_t {
  return (value >> index) & 1UL;
}

void state::print() const {
  for (auto i = bits; i > 0UL; i--) {
    auto bit = (value >> (i - 1UL)) & 1UL;
    std::cout << bit;
  }
  std::cout << std::endl;
}

void state::set(uint64_t new_value) {
  auto mask = (1UL << bits) - 1UL;
  value = new_value & mask;
}

void state::set_bit(uint64_t index, uint64_t bit_value) {
  assert(bit_value == 0UL || bit_value == 1UL);
  assert(index < bits);
  auto rhs = bit_value << index;
  auto mask = ~(1UL << index);
  auto lhs = value & mask;
  value = lhs | rhs;
}

void state::switch_bit(uint64_t index) {
  assert(index < bits);
  auto mask = 1UL << index;
  value ^= mask;
}
