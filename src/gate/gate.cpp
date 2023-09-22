#include "gate.hpp"
#include <random>

std::vector<uint64_t> random_unique_vector(uint64_t vector_size,
                                           uint64_t range_upper,
                                           std::mt19937_64 mrnd) {

  std::vector<uint64_t> numbers(range_upper);
  std::iota(numbers.begin(), numbers.end(), 0);

  std::shuffle(numbers.begin(), numbers.end(), mrnd);

  std::vector<uint64_t> result(vector_size);
  std::copy(numbers.begin(),
            numbers.begin() + static_cast<ptrdiff_t>(vector_size),
            result.begin()); // Copy the first k numbers

  return result;
}

gate::gate(uint64_t size, std::vector<uint64_t> controls, uint64_t target)
    : bits_num_(size), controls_([&controls]() {
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

gate::gate(uint64_t size, std::mt19937_64 mrnd) {
  const auto controls_num = mrnd() % size + 1; // actual controls + target;
  auto controls = random_unique_vector(controls_num, size, mrnd);
  const auto target = controls.back();
  controls.pop_back();

  sort(controls.begin(), controls.end());

  bits_num_ = size;
  controls_ = controls;
  target_ = target;
  control_mask_ = [controls]() {
    uint64_t mask = 0UL;
    for (auto control : controls) {
      mask |= 1 << control;
    }
    return mask;
  }();
  target_mask_ = (1 << target);
}

auto gate::bits_num() const -> uint64_t { return bits_num_; }

auto gate::controls() const -> std::vector<uint64_t> { return controls_; }

auto gate::target() const -> uint64_t { return target_; }

auto gate::control_mask() const -> uint64_t { return control_mask_; }

auto gate::target_mask() const -> uint64_t { return target_mask_; }

auto gate::apply(uint64_t row) const -> uint64_t {
  if ((row & control_mask_) == control_mask_) {
    return row ^ target_mask_;
  }
  else {
    return row;
  }
}

auto gate::apply(state &s) const -> void {
  assert(s.bits_num() == bits_num());
  s.set_value(apply(s.value()));
}

auto gate::apply_back(truth_table &tt) const -> void {
  assert(tt.bits_num() == bits_num());
  for (auto index = 0UL; index < tt.size(); index++) {
    tt.set_row(index, apply(tt.row(index)));
  }
}

auto gate::apply_front(truth_table &tt) const -> void {
  assert(tt.bits_num() == bits_num());
  for (auto index = 0UL; index < tt.size(); index++) {
    bool is_control_set = (index & control_mask_) == control_mask_;
    bool is_target_set = (index & target_mask_) == target_mask_;
    if (is_control_set && is_target_set) {
      auto next_index = index ^ target_mask_;
      tt.swap(index, next_index);
    }
  }
}

auto gate::print() const -> void {
  std::cout << "Size:     " << bits_num_ << std::endl;
  std::cout << "Target:   " << target_ << std::endl;
  std::cout << "Controls: " << std::endl;
  for (auto c : controls_) {
    std::cout << c << ", ";
  }
  std::cout << "T mask:   " << target_mask_ << std::endl;
  std::cout << "C mask:   " << control_mask_ << std::endl;
  std::cout << std::endl;
}
