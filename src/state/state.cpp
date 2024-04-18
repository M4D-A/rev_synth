#include "state.hpp"
#include "iostream"
#include <cassert>
#include <cstdint>
#include <random>
#include <stdexcept>
#include <strings.h>

auto state::check_size(uint64_t size) noexcept(false) -> uint64_t {
  if (size == 0) {
    throw std::invalid_argument("Size of state must be a positive number");
  }
  if (size > MAX_SIZE) {
    throw std::invalid_argument("Size of state cannot exceed MAX_SIZE");
  }
  return size;
}

auto state::mask(uint64_t size) -> uint64_t {
  check_size(size);
  return MAX_MASK >> (MAX_SIZE - size);
}

auto state::check_value(uint64_t value) noexcept(false) -> uint64_t {
  if (value > _mask) {
    throw std::invalid_argument("Value exceeds size of state");
  }
  return value;
}

state::state(uint64_t size, uint64_t value)
    : _size(check_size(size)), _mask(mask(size)), _value(check_value(value)) {}

state::state(uint64_t size, std::mt19937_64 mrnd)
    : _size(check_size(size)), _mask(mask(size)), _value(mrnd() & _mask) {}

auto state::size() const noexcept -> uint64_t { return _size; }

auto state::mask() const noexcept -> uint64_t { return _mask; }

auto state::value() const noexcept -> uint64_t { return _value; }

auto state::bit_value(uint64_t index) const noexcept -> uint64_t { return (_value >> index) & 1UL; }

auto state::zeroes() const noexcept -> std::vector<uint64_t> {
  auto retval = std::vector<uint64_t>();
  for (auto i = 0UL; i < _size; i++) {
    if (bit_value(i) == 0) {
      retval.push_back(i);
    }
  }
  return retval;
}

auto state::ones() const noexcept -> std::vector<uint64_t> {
  auto retval = std::vector<uint64_t>();
  for (auto i = 0UL; i < _size; i++) {
    if (bit_value(i) == 1) {
      retval.push_back(i);
    }
  }
  return retval;
}

auto state::set_value(uint64_t value) -> state & {
  _value = check_value(value);
  return *this;
}

auto state::set_bit(uint64_t index, uint64_t bit_value) -> state & {
  if (index >= _size) {
    throw std::invalid_argument("Bit index has to be smaller than state size");
  }
  if (bit_value > 1) {
    throw std::invalid_argument("Bit value has to be equal to 0 or 1");
  }
  auto rhs = bit_value << index;
  auto mask = ~(1UL << index);
  auto lhs = _value & mask;
  _value = lhs | rhs;
  return *this;
}

auto state::switch_bit(uint64_t index) -> state & {
  if (index >= _size) {
    throw std::invalid_argument("Bit index has to be smaller than state size");
  }
  auto mask = 1UL << index;
  _value ^= mask;
  return *this;
}

auto state::negate() noexcept -> state & {
  _value = (~_value) & _mask;
  return *this;
}

auto state::operator==(const uint64_t &rhs) const -> bool { return rhs == _value; }

auto state::operator[](const uint64_t index) const -> uint64_t { return bit_value(index); }

auto state::operator^(const state &rhs) const -> state {
  assert(size() == rhs.size());
  auto new_value = value() ^ rhs.value();
  return {size(), new_value};
}

auto state::operator&(const state &rhs) const -> state {
  assert(size() == rhs.size());
  auto new_value = value() & rhs.value();
  return {size(), new_value};
}

auto state::operator|(const state &rhs) const -> state {
  assert(size() == rhs.size());
  auto new_value = value() | rhs.value();
  return {size(), new_value};
}

state::operator uint64_t() const { return value(); }

auto state::print() const -> void {
  for (auto i = 0UL; i < _size; i++) {
    auto bit = (_value >> (i - 1UL)) & 1UL;
    std::cout << bit;
  }
  std::cout << '\n';
}
