#include "gate.hpp"
#include "utils/utils.hpp"
#include <iostream>
#include <ostream>
#include <random>
#include <stdexcept>

auto gate::sort_controls(std::vector<uint64_t> &controls) {
  std::sort(controls.begin(), controls.end());
  return controls;
}

auto gate::control_mask(const std::vector<uint64_t> &controls) {
  uint64_t mask = 0UL;
  for (auto control : controls) {
    mask |= 1UL << control;
  }
  return mask;
}

auto gate::target_mask(uint64_t target) { return 1UL << target; }

gate::gate(uint64_t size, std::vector<uint64_t> controls, uint64_t target)
    : _size(size), _controls(sort_controls(controls)), _target(target),
      _control_mask(control_mask(controls)), _target_mask(target_mask(target)) {}

gate::gate(uint64_t size, std::mt19937_64 mrnd) : _size(size) {
  const auto taps_num = mrnd() % size + 1; // actual controls_num + target;
  auto controls = random_unique_vector(taps_num, size, mrnd);
  const auto target = controls.back();
  controls.pop_back();
  sort(controls.begin(), controls.end());

  _controls = controls;
  _control_mask = control_mask(controls);
  _target = target;
  _target_mask = target_mask(target);
}

auto gate::size() const noexcept -> uint64_t { return _size; }

auto gate::controls() const noexcept -> std::vector<uint64_t> { return _controls; }

auto gate::controls_num() const noexcept -> uint64_t { return _controls.size(); }

auto gate::target() const noexcept -> uint64_t { return _target; }

auto gate::control_mask() const noexcept -> uint64_t { return _control_mask; }

auto gate::target_mask() const noexcept -> uint64_t { return _target_mask; }

auto gate::apply(uint64_t row) const noexcept -> uint64_t {
  if ((row & _control_mask) == _control_mask) {
    return row ^ _target_mask;
  }
  else {
    return row;
  }
}

auto gate::apply(state &s) const -> void {
  if (s.size() != size()) {
    throw std::invalid_argument("Cannot apply gate to state of different size");
  }
  s.set_value(apply(s.value()));
}

auto gate::apply_back(truth_table &tt) const -> void {
  if (tt.size() != size()) {
    throw std::invalid_argument("Cannot apply gate to truth_table of different size");
  }
  for (auto index = 0UL; auto row : tt) {
    tt.set_row(index++, apply(row));
  }
}

auto gate::apply_front(truth_table &tt) const -> void {
  if (tt.size() != size()) {
    throw std::invalid_argument("Cannot apply gate to truth_table of different size");
  }
  for (auto index = 0UL; index < tt.length(); index++) {
    bool is_control_set = (index & _control_mask) == _control_mask;
    bool is_target_set = (index & _target_mask) == _target_mask;
    if (is_control_set && is_target_set) {
      auto next_index = index ^ _target_mask;
      tt.swap(index, next_index);
    }
  }
}

auto gate::print() const -> void {
  std::cout << "Size:     " << _size << std::endl;
  std::cout << "Target:   " << _target << std::endl;
  std::cout << "Controls: " << std::endl;
  for (auto c : _controls) {
    std::cout << c << ", ";
  }
  std::cout << "T mask:   " << _target_mask << std::endl;
  std::cout << "C mask:   " << _control_mask << std::endl;
  std::cout << std::endl;
}
