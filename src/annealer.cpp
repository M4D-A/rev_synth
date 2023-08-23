#include "annealer.hpp"
#include <random>

uint64_t cost(const std::vector<uint8_t> &lhs,
              const std::vector<uint8_t> &rhs) {
  uint64_t size = std::min(lhs.size(), rhs.size());
  uint64_t matchings = 0;
  for (uint64_t i = 0; i < size; i++) {
    if (lhs[i] == rhs[i]) {
      matchings++;
    }
  }
  return size - matchings;
}

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

circuit anneal(const uint64_t size, const std::vector<uint8_t> &target_tt,
               uint64_t seed) {
  circuit candidate(size);
  std::mt19937_64 mrnd(seed);
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  const auto INIT_TEMP = 10.0;
  const auto MIN_TEMP = 0.01;
  const auto TEMP_RATE = 0.8;
  const auto ROUNDS = 100UL;

  double current_temp = INIT_TEMP;
  while (current_temp >= MIN_TEMP) {
    for (auto i = 0UL; i < ROUNDS; i++) {
      auto mode = mrnd() % 2;
      if (candidate.target_masks_.size() == 0)
        mode = 0;
      auto side = mrnd() % 2;

      if (mode == 0) {
        auto gate_size = mrnd() % size + 1;
        auto gate_bits = random_unique_vector(gate_size, size, mrnd);
        auto target_bit = gate_bits.back();
        gate_bits.pop_back();
        auto old_cost =
            static_cast<int64_t>(cost(candidate.truth_table_, target_tt));
        if (side == 0) {
          candidate.push_back(target_bit, gate_bits);
        }
        else {
          candidate.push_front(target_bit, gate_bits);
        }
        auto new_cost =
            static_cast<int64_t>(cost(candidate.truth_table_, target_tt));
        auto delta = static_cast<double>(new_cost - old_cost);
        std::cout << delta << std::endl;

        if (delta < 0) {
          continue;
        }
        else {
          auto threshold = pow(2.0, -delta / current_temp);
          auto result = dist(mrnd);
          if (result < threshold) {
            continue;
          }
          else {
            if (side == 0) {
              candidate.pop_back();
            }
            else {
              candidate.pop_front();
            }
          }
        }
      }
      else {
        uint8_t old_target_mask;
        uint8_t old_control_mask;
        if (side == 0) {
          old_target_mask = candidate.target_masks_.back();
          old_control_mask = candidate.control_masks_.back();
        }
        else {
          old_target_mask = candidate.target_masks_.front();
          old_control_mask = candidate.control_masks_.front();
        }
        auto old_cost =
            static_cast<int64_t>(cost(candidate.truth_table_, target_tt));
        if (side == 0) {
          candidate.pop_back();
        }
        else {
          candidate.pop_front();
        }
        auto new_cost =
            static_cast<int64_t>(cost(candidate.truth_table_, target_tt));
        auto delta = static_cast<double>(new_cost - old_cost);
        std::cout << delta << std::endl;

        if (delta < 0) {
          continue;
        }
        else {
          auto threshold = pow(2.0, -delta / current_temp);
          auto result = dist(mrnd);
          if (result < threshold) {
            continue;
          }
          else {
            if (side == 0) {
              candidate.push_back(old_target_mask, old_control_mask);
            }
            else {
              candidate.push_front(old_target_mask, old_control_mask);
            }
          }
        }
      }
    }
    current_temp *= TEMP_RATE;
  }
  return candidate;
}
