#pragma once
#include "gate/gate.hpp"
#include <cassert>
#include <cstdint>
#include <deque>
#include <random>

class circuit {
  std::deque<gate> gates_;
  truth_table output_tt_;
  uint64_t bits_num_;

public:
  circuit(uint64_t bits_num);
  circuit(uint64_t bits_num, uint64_t gates_num, std::mt19937_64);

  auto bits_num() const -> uint64_t;
  auto gates_num() const -> uint64_t;
  auto gates() const -> const std::deque<gate> &;
  auto output_tt() const -> const truth_table &;

  auto apply(uint64_t row) const -> uint64_t;
  auto apply(state &s) const -> void;
  auto apply_back(truth_table &tt) const -> void;
  auto apply_front(truth_table &tt) const -> void;

  auto push_back(gate new_gate) -> circuit &;
  auto push_front(gate new_gate) -> circuit &;
  auto push_back(const circuit &new_circuit) -> circuit &;
  auto push_front(const circuit &new_circuit) -> circuit &;

  auto operator==(const circuit &rhs) const -> bool = default;
  auto operator[](uint64_t index) const -> gate;

  template <typename T> auto operator+=(const T &rhs) -> circuit &;
  template <typename T>
  friend auto operator+(circuit lhs, const T &rhs) -> circuit;

  auto print() -> void;
  auto graph() -> void;
};
