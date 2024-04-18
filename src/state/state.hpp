#pragma once
#include <cstdint>
#include <limits>
#include <vector>

class state {
  uint64_t _size;
  uint64_t _mask;
  uint64_t _value;

public:
  static const auto MAX_MASK = std::numeric_limits<uint64_t>::max();
  static const auto MAX_SIZE = 64UL;

  static auto mask(uint64_t size) -> uint64_t;
  static auto check_size(uint64_t size) noexcept(false) -> uint64_t;
  auto check_value(uint64_t value) noexcept(false) -> uint64_t;

  state(uint64_t size, uint64_t value);

  [[nodiscard]] auto size() const noexcept -> uint64_t;
  [[nodiscard]] auto mask() const noexcept -> uint64_t;
  [[nodiscard]] auto value() const noexcept -> uint64_t;
  [[nodiscard]] auto bit_value(uint64_t index) const noexcept -> uint64_t;
  [[nodiscard]] auto zeroes() const noexcept -> std::vector<uint64_t>;
  [[nodiscard]] auto ones() const noexcept -> std::vector<uint64_t>;

  auto set_value(uint64_t value) -> state &;
  auto set_bit(uint64_t index, uint64_t bit_value) -> state &;
  auto switch_bit(uint64_t index) -> state &;
  auto negate() noexcept -> state &;

  auto operator==(const state &rhs) const -> bool = default;
  auto operator==(const uint64_t &rhs) const -> bool;

  auto operator^(const state &rhs) const -> state;
  auto operator&(const state &rhs) const -> state;
  auto operator|(const state &rhs) const -> state;

  auto operator[](uint64_t index) const -> uint64_t;

  explicit operator uint64_t() const;

  auto print() const -> void;
};
