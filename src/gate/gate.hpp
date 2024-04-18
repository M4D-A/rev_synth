#pragma once
#include "state/state.hpp"
#include "truth_table/truth_table.hpp"
#include <cstdint>
#include <vector>

class gate {
  uint64_t _size;
  std::vector<uint64_t> _controls;
  uint64_t _target;
  uint64_t _control_mask;
  uint64_t _target_mask;

public:
  static auto sort_controls(std::vector<uint64_t> &controls);
  static auto control_mask(const std::vector<uint64_t> &contrsols);
  static auto target_mask(uint64_t target);

  gate(uint64_t size, std::vector<uint64_t> controls, uint64_t target);
  gate(uint64_t size, std::mt19937_64 mrnd);

  [[nodiscard]] auto size() const noexcept -> uint64_t;
  [[nodiscard]] auto controls() const noexcept -> std::vector<uint64_t>;
  [[nodiscard]] auto controls_num() const noexcept -> uint64_t;
  [[nodiscard]] auto target() const noexcept -> uint64_t;
  [[nodiscard]] auto control_mask() const noexcept -> uint64_t;
  [[nodiscard]] auto target_mask() const noexcept -> uint64_t;

  [[nodiscard]] auto apply(uint64_t row) const noexcept -> uint64_t;
  auto apply(state &s) const -> void;
  auto apply_back(truth_table &tt) const -> void;
  auto apply_front(truth_table &tt) const -> void;

  auto operator==(const gate &) const -> bool = default;
  auto print() const -> void;
};
