#pragma once
#include <cstdint>
#include <deque>
#include <iostream>
#include <numeric>
#include <vector>

struct circuit {
  const uint64_t size_;
  std::deque<uint8_t> target_masks_;
  std::deque<uint8_t> control_masks_;
  std::vector<uint8_t> truth_table_;

  circuit(uint64_t size);
  void push_back(uint64_t target_id, const std::vector<uint64_t> &control_ids);
  void push_back(uint8_t target_mask, uint8_t control_mask);
  void pop_back();
  void push_front(uint64_t target_id, const std::vector<uint64_t> &control_ids);
  void push_front(uint8_t target_mask, uint8_t control_mask);
  void pop_front();
  void print();

private:
  uint8_t ids_to_mask(const std::vector<uint64_t> &ids);
};
