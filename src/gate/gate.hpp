#pragma once
#include "state/state.hpp"
#include "truth_table/truth_table.hpp"
#include <cassert>
#include <cstdint>
#include <vector>

class gate {
  const uint64_t size_;
  const std::vector<uint64_t> controls_;
  const uint64_t target_;
  const uint64_t control_mask_;
  const uint64_t target_mask_;

public:
  enum application_side { input, output };
  gate(uint64_t size, std::vector<uint64_t> controls, uint64_t target);

  auto get_size() const -> uint64_t;
  auto get_controls() const -> std::vector<uint64_t>;
  auto get_target() const -> uint64_t;
  auto get_control_mask() const -> uint64_t;
  auto get_target_mask() const -> uint64_t;

  auto apply(uint64_t row) const -> uint64_t;
  auto apply(state &s) const -> void;
  auto apply(truth_table &tt, application_side = output) const -> void;

  auto print() const -> void;
};
