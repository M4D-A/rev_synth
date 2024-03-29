#pragma once
#include "state/state.hpp"
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

class truth_table {
private:
  uint64_t bits_num_;
  uint64_t bits_mask_;
  std::vector<uint64_t> data_;

public:
  truth_table(uint64_t bits_num);

  auto bits_num() const -> uint64_t;
  auto size() const -> uint64_t;

  auto bits_mask() const -> uint64_t;
  auto data() const -> std::vector<uint64_t>;
  auto row(uint64_t index) const -> uint64_t;

  auto set_data(std::vector<uint64_t> data) -> truth_table &;
  auto set_row(uint64_t index, uint64_t value) -> truth_table &;
  auto inverse() -> truth_table &;
  auto shuffle(std::mt19937_64 &mrnd) -> truth_table &;
  auto swap(uint64_t index_1, uint64_t index_2) -> truth_table &;
  auto next_permutation() -> bool;

  auto operator[](uint64_t index) const -> uint64_t;
  auto operator[](uint64_t index) -> uint64_t &;

  auto operator==(const truth_table &rhs) const -> bool = default;
  auto operator+(const truth_table &rhs) const -> truth_table;
  auto operator+=(const truth_table &rhs) -> truth_table &;

  auto print() const -> void;
};
