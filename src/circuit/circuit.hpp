#pragma once
#include "gate/gate.hpp"
#include <cassert>
#include <cstdint>
#include <deque>

class circuit {
  std::deque<gate> gates_;
  truth_table output_;
  const uint64_t width_;

public:
  circuit(uint64_t width_);

  auto get_width() const -> uint64_t;
  auto get_gates_num() const -> uint64_t;
  auto get_gates() const -> const std::deque<const gate> &;
  auto get_truth_table() const -> const truth_table &;

  auto apply(uint64_t row) const -> uint64_t;
  auto apply(state &s) const -> void;
  auto apply_back(truth_table &tt) const -> void;
  auto apply_front(truth_table &tt) const -> void;
  auto push_back(gate new_gate) -> void;
  auto push_front(gate new_gate) -> void;
};
