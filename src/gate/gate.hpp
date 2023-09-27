#pragma once
#include "state/state.hpp"
#include "truth_table/truth_table.hpp"
#include <cassert>
#include <cstdint>
#include <vector>

class gate {
  uint64_t bits_num_;
  std::vector<uint64_t> controls_;
  uint64_t target_;
  uint64_t control_mask_;
  uint64_t target_mask_;

public:
  gate(uint64_t bits_num, std::vector<uint64_t> controls, uint64_t target);
  gate(uint64_t bits_num, std::mt19937_64 mrnd);

  auto bits_num() const -> uint64_t;
  auto controls() const -> std::vector<uint64_t>;
  auto controls_num() const -> uint64_t;
  auto target() const -> uint64_t;
  auto control_mask() const -> uint64_t;
  auto target_mask() const -> uint64_t;

  auto apply(uint64_t row) const -> uint64_t;
  auto apply(state &s) const -> void;
  auto apply_back(truth_table &tt) const -> void;
  auto apply_front(truth_table &tt) const -> void;

  auto operator==(const gate &) const -> bool = default;
  auto print() const -> void;
};
