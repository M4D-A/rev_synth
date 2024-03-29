#pragma once
#include <cstdint>
#include <iostream>
#include <vector>

class state {
  uint64_t bits_num_;
  uint64_t bits_mask_;
  uint64_t value_;

public:
  state(uint64_t bits_num, uint64_t value);

  auto bits_num() const -> uint64_t;
  auto bits_mask() const -> uint64_t;
  auto value() const -> uint64_t;
  auto bit_value(uint64_t index) const -> uint64_t;
  auto zeroes() const -> std::vector<uint64_t>;
  auto ones() const -> std::vector<uint64_t>;

  auto set_value(uint64_t value) -> state &;
  auto set_bit(uint64_t index, uint64_t bit_value) -> state &;
  auto switch_bit(uint64_t index) -> state &;
  auto negate() -> state &;

  auto operator==(const state &rhs) const -> bool = default;
  auto operator==(const uint64_t &rhs) const -> bool;

  auto operator^(const state &rhs) const -> state;
  auto operator&(const state &rhs) const -> state;
  auto operator|(const state &rhs) const -> state;

  auto operator[](const uint64_t index) const -> uint64_t;

  operator uint64_t() const;

  auto print() const -> void;
};
