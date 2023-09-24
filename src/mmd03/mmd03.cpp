#include "mmd03.hpp"
auto mmd03::synthesize(truth_table target_tt) -> circuit {
  auto bits_num = target_tt.bits_num();
  auto circ = circuit(bits_num);

  // f(0) = 0
  auto r0_state = state(bits_num, target_tt[0]);
  auto ones = r0_state.ones();

  for (auto one : ones) {
    auto new_gate = gate(bits_num, {}, one);
    new_gate.apply_back(target_tt);
    circ.push_front(new_gate);
  }

  // f(i) = i : i >= 1
  for (auto i = 1UL; i < target_tt.size(); i++) {
    // setting to ones zeroes that should be ones
    auto ri = target_tt[i];
    auto controls = state(bits_num, ri).ones();
    auto zero_to_one_mask = ~ri & i;
    auto zero_to_one_ids = state(bits_num, zero_to_one_mask).ones();
    for (auto id : zero_to_one_ids) {
      auto new_gate = gate(bits_num, controls, id);
      new_gate.apply_back(target_tt);
      circ.push_front(new_gate);
    }

    // setting to zeroes ones that should be zeroes
    ri = target_tt[i];
    auto one_to_zero_mask = ri & ~i;
    auto one_to_zero_ids = state(bits_num, one_to_zero_mask).ones();
    auto correct_ones_mask = ri & i;
    auto correct_ones_ids = state(bits_num, correct_ones_mask).ones();
    for (auto id : one_to_zero_ids) {
      auto new_gate = gate(bits_num, correct_ones_ids, id);
      new_gate.apply_back(target_tt);
      circ.push_front(new_gate);
    }
  }
  return circ;
}
