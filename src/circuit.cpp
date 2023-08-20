#include "circuit.hpp"
#include <algorithm>
#include <iterator>

circuit::circuit(uint64_t size) : size_(size) {
  truth_table_.resize(1UL << size);
  std::iota(truth_table_.begin(), truth_table_.end(), 0UL);
}

void circuit::push_back(uint8_t target_mask, uint8_t control_mask) {
  target_masks_.push_back(target_mask);
  control_masks_.push_back(control_mask);

  for (auto &row : truth_table_) {
    if ((row & control_mask) == control_mask) {
      row ^= target_mask;
    }
  }
}

void circuit::push_back(uint64_t target_id, std::vector<uint64_t> control_ids) {
  uint8_t target_mask = static_cast<uint8_t>(1UL << target_id);
  uint8_t control_mask = 0UL;
  for (auto control : control_ids) {
    control_mask |= static_cast<uint8_t>(1UL << control);
  }
  push_back(target_mask, control_mask);
}

void circuit::pop_back() {
  auto target_mask = target_masks_.back();
  auto control_mask = control_masks_.back();
  target_masks_.pop_back();
  control_masks_.pop_back();

  for (auto &row : truth_table_) {
    if ((row & control_mask) == control_mask) {
      row ^= target_mask;
    }
  }
}

void circuit::push_front(uint8_t target_mask, uint8_t control_mask) {
  target_masks_.push_front(target_mask);
  control_masks_.push_front(control_mask);
  for (auto index = 0UL; index < truth_table_.size(); index++) {
    bool is_control_set = (index & control_mask) == control_mask;
    bool is_target_set = (index & target_mask) == target_mask;
    if (is_control_set && is_target_set) {
      auto next_index = index ^ target_mask;
      std::swap(truth_table_[index], truth_table_[next_index]);
    }
  }
}

void circuit::push_front(uint64_t target_id,
                         std::vector<uint64_t> control_ids) {
  uint8_t target_mask = static_cast<uint8_t>(1UL << target_id);
  uint8_t control_mask = 0UL;
  for (auto control : control_ids) {
    control_mask |= static_cast<uint8_t>(1UL << control);
  }
  push_front(target_mask, control_mask);
}

void circuit::print() {
  for (uint64_t i = 0UL; i < target_masks_.size(); i++) {
    for (uint64_t s = 0; s < size_; s++) {
      if ((target_masks_[i] >> s) & 1UL) {
        printf("%ld <- ", s);
        break;
      }
    }

    for (uint64_t s = 0; s < size_; s++) {
      if ((control_masks_[i] >> s) & 1UL) {
        printf("%ld, ", s);
      }
    }
    printf("\n");
  }

  for (auto row : truth_table_) {
    for (auto i = size_; i > 0UL; i--) {
      auto bit = (row >> (i - 1UL)) & 1UL;
      std::cout << bit;
    }
    std::cout << std::endl;
  }
}
