#include "annealer.hpp"
#include <random>

bool random_pass(const double threshold, std::mt19937_64 &mrnd) {
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  auto result = dist(mrnd);
  return result < threshold;
}

circuit anneal(const uint64_t size, const std::vector<uint8_t> &target_tt,
               uint64_t seed) {
  circuit candidate(size);
  std::mt19937_64 mrnd(seed);

  const auto INIT_TEMP = 4.0;
  const auto MIN_TEMP = 0.1;
  const auto TEMP_RATE = 0.99;
  const auto ROUNDS = 1000000UL;
  const auto TARGET_SIZE = 20;

  double current_temp = INIT_TEMP;
  while (current_temp >= MIN_TEMP) {
    for (auto i = 0UL; i < ROUNDS; i++) {
      auto side = random_pass(0.5, mrnd);

      auto gates_num = candidate.target_masks_.size();
      auto mode_threshold = (double)gates_num / (2.0 * (double)TARGET_SIZE);
      auto mode = random_pass(mode_threshold, mrnd);

      auto old_cost = candidate.distance(target_tt);
      if (old_cost == 0) {
        return candidate;
      }

      if (mode == false) {
        if (side)
          candidate.push_back_random(mrnd);
        else
          candidate.push_front_random(mrnd);

        auto new_cost = candidate.distance(target_tt);
        auto delta = static_cast<double>(new_cost - old_cost);

        auto threshold = pow(2.0, -delta / current_temp);
        if (delta >= 0 and !random_pass(threshold, mrnd)) {
          if (side)
            candidate.pop_back();
          else
            candidate.pop_front();
        }
      }
      else {
        uint8_t old_target_mask;
        uint8_t old_control_mask;
        if (side) {
          old_target_mask = candidate.target_masks_.back();
          old_control_mask = candidate.control_masks_.back();
        }
        else {
          old_target_mask = candidate.target_masks_.front();
          old_control_mask = candidate.control_masks_.front();
        }

        if (side)
          candidate.pop_back();
        else
          candidate.pop_front();

        auto new_cost = candidate.distance(target_tt);
        auto delta = static_cast<double>(new_cost - old_cost);

        auto threshold = pow(2.0, -delta / current_temp);
        if (delta >= 0 and !random_pass(threshold, mrnd)) {
          if (side == 0) {
            candidate.push_back(old_target_mask, old_control_mask);
          }
          else {
            candidate.push_front(old_target_mask, old_control_mask);
          }
        }
      }
    }
    current_temp *= TEMP_RATE;
  }
  return candidate;
}
