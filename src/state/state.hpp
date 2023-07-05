#include <cstdint>
#include <iostream>

class state {
  const uint64_t bits_num_;
  const uint64_t bits_mask_;
  uint64_t value_;

public:
  state(uint64_t bits_num, uint64_t value = 0UL);

  auto get_bits_num() const -> uint64_t;
  auto get_bits_mask() const -> uint64_t;
  auto get_value() const -> uint64_t;
  auto get_bit(uint64_t index) const -> uint64_t;

  auto set_value(uint64_t value) -> void;
  auto set_bit(uint64_t index, uint64_t bit_value) -> void;
  auto switch_bit(uint64_t index) -> void;
  auto negate() -> void;

  auto operator==(const state &rhs) const -> bool = default;
  auto operator==(const uint64_t &rhs) const -> bool;

  auto operator[](const uint64_t index) const -> uint64_t;

  auto print() const -> void;
};
