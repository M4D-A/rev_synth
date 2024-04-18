#pragma once
#include "state/state.hpp"
#include <cstdint>
#include <random>
#include <vector>

class truth_table {
private:
  uint64_t _size;
  uint64_t _mask;
  std::vector<uint64_t> _data;

public:
  explicit truth_table(uint64_t bits_num);

  [[nodiscard]] auto size() const noexcept -> uint64_t;
  [[nodiscard]] auto length() const noexcept -> uint64_t;

  [[nodiscard]] auto mask() const noexcept -> uint64_t;
  [[nodiscard]] auto data() const noexcept -> const std::vector<uint64_t> &;
  [[nodiscard]] auto row(uint64_t index) const -> uint64_t;
  [[nodiscard]] auto state(uint64_t index) const -> state;

  auto set_data(std::vector<uint64_t> data) -> truth_table &;
  auto set_row(uint64_t index, uint64_t value) -> truth_table &;
  auto inverse() -> truth_table &;
  auto shuffle(std::mt19937_64 &mrnd) -> truth_table &;
  auto swap(uint64_t index_1, uint64_t index_2) noexcept(false) -> truth_table &;
  auto next_permutation() -> bool;

  auto begin() -> std::vector<uint64_t>::iterator;
  auto end() -> std::vector<uint64_t>::iterator;

  auto operator[](uint64_t index) const -> uint64_t;
  auto operator[](uint64_t index) -> uint64_t &;

  auto operator==(const truth_table &rhs) const -> bool = default;
  auto operator+(const truth_table &rhs) const -> truth_table;
  auto operator+=(const truth_table &rhs) -> truth_table &;

  auto print() const -> void;
};
