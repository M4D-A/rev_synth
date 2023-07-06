#include "gate.hpp"

gate::gate(uint64_t size, std::vector<uint64_t> controls, uint64_t target)
    : size_(size), controls_([&controls]() {
        std::sort(controls.begin(), controls.end());
        return controls;
      }()),
      target_(target), control_mask_([controls]() {
        uint64_t mask = 0UL;
        for (auto control : controls) {
          mask |= 1 << control;
        }
        return mask;
      }()),
      target_mask_(1 << target) {
  assert(std::all_of(controls.begin(), controls.end(),
                     [size](auto x) { return x < size; }));
  assert(target < size);
}

auto gate::get_size() const -> uint64_t { return size_; }

auto gate::get_controls() const -> std::vector<uint64_t> { return controls_; }

auto gate::get_target() const -> uint64_t { return target_; }

auto gate::get_control_mask() const -> uint64_t { return control_mask_; }

auto gate::get_target_mask() const -> uint64_t { return target_mask_; }

auto gate::apply(uint64_t row) const -> uint64_t {
  if ((row & control_mask_) == control_mask_) {
    return row ^ target_mask_;
  }
  else {
    return row;
  }
}

auto gate::apply(state &s) const -> void {
  assert(s.get_bits_num() == get_size());
  s.set_value(apply(s.get_value()));
}

auto gate::apply(truth_table &tt, application_side aps) const -> void {
  assert(tt.get_bits_num() == get_size());
  if (aps == output) {
    for (auto index = 0UL; index < tt.get_size(); index++) {
      tt.set_row(index, apply(tt.get_row(index)));
    }
  }
  else if (aps == input) {
    for (auto index = 0UL; index < tt.get_size(); index++) {
      bool is_control_set = (index & control_mask_) == control_mask_;
      bool is_target_set = (index & target_mask_) == target_mask_;
      if (is_control_set && is_target_set) {
        auto next_index = index ^ target_mask_;
        tt.swap(index, next_index);
      }
    }
  }
}

auto gate::print() const -> void {
  std::cout << "Size:     " << size_ << std::endl;
  std::cout << "Target:   " << target_ << std::endl;
  std::cout << "Controls: " << std::endl;
  for (auto c : controls_) {
    std::cout << c << ", ";
  }
  std::cout << std::endl;
  std::cout << "T mask:   " << target_mask_ << std::endl;
  std::cout << "C mask:   " << control_mask_ << std::endl;
}
