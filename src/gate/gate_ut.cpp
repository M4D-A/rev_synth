#include "gate.hpp"
#include "utils/utils.hpp"
#include <bits/fs_fwd.h>
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>
#include <ranges>

namespace gate_ut {
const auto EPOCHS = 10000;
const auto max_size = 12;

} // namespace gate_ut

using namespace gate_ut;

TEST_CASE("gate constructors and getters", "[gate], [ctors], [getters]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_size))));

  auto taps_num = mrnd() % size + 1;
  auto controls = random_unique_vector(taps_num, size, mrnd);
  auto target = controls.back();
  controls.pop_back();
  std::shuffle(controls.begin(), controls.end(), mrnd);

  auto tested_1 = gate(size, controls, target);
  REQUIRE(tested_1.size() == size);

  std::sort(controls.begin(), controls.end());
  REQUIRE(tested_1.controls() == controls);

  std::shuffle(controls.begin(), controls.end(), mrnd);
  auto tested_2 = gate(size, controls, target);
  REQUIRE(tested_1 == tested_2);

  auto control_mask = tested_1.control_mask();
  for (auto i = 0UL; i < max_size; i++) {
    const auto is_control_in_vector =
        std::find(controls.begin(), controls.end(), i) != controls.end();
    const auto is_control_bit_set = static_cast<bool>((control_mask >> i) & 1UL);
    REQUIRE(is_control_bit_set == is_control_in_vector);
  }

  auto target_mask = tested_1.target_mask();
  for (auto i = 0UL; i < max_size; i++) {
    const auto is_target_bit_set = static_cast<bool>((target_mask >> i) & 1UL);
    const auto is_target_bit = i == target;
    REQUIRE(is_target_bit_set == is_target_bit);
  }
}

TEST_CASE("gate apply", "[gate], [apply]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_size))));

  auto tested = gate(size, mrnd);
  auto controls = tested.controls();
  auto target = tested.target();

  SECTION("row") {
    auto row = mrnd() & state::mask(size);
    auto new_row = tested.apply(row);
    auto new_new_row = tested.apply(new_row);
    REQUIRE(new_new_row == row);
    bool is_control_set =
        std::all_of(controls.begin(), controls.end(), [row](auto i) { return (row >> i) & 1UL; });
    if (is_control_set) {
      REQUIRE((row ^ new_row) == 1UL << target);
    }
    else {
      REQUIRE(row == new_row);
    }
  }

  SECTION("state") {
    auto test_state = state(size, mrnd);
    auto target_value = tested.apply(test_state.value());

    INFO(size);
    INFO(test_state.value());
    INFO(target_value);

    tested.apply(test_state);
    REQUIRE(test_state == target_value);
  }

  SECTION("truth table") {
    auto test_tt = truth_table(size);
    tested.apply_back(test_tt);

    for (auto input = 0UL; auto row : test_tt) {
      REQUIRE(row == tested.apply(input++));
    }
  }

  SECTION("truth table on input") {
    auto target_tt = truth_table(size);
    tested.apply_front(target_tt);
    for (auto input = 0UL; input < target_tt.length(); input++) {
      const auto output = target_tt[input];
      REQUIRE(output == tested.apply(input));
    }
  }
}

TEST_CASE("complex gate apply", "[gate], [apply]") {

  std::mt19937_64 mrnd;
  const auto bits = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_size))));

  SECTION("two gates") {
    std::vector<gate> tested_gates;
    tested_gates.reserve(2UL);
    for (auto i = 0UL; i < 2UL; i++) {
      tested_gates.emplace_back(bits, mrnd);
    }

    auto tt_out_out = truth_table(bits);
    auto tt_out_inp = truth_table(bits);
    auto tt_inp_out = truth_table(bits);
    auto tt_inp_inp = truth_table(bits);

    tested_gates[0].apply_back(tt_out_out);
    tested_gates[1].apply_back(tt_out_out);

    tested_gates[1].apply_back(tt_out_inp);
    tested_gates[0].apply_front(tt_out_inp);

    tested_gates[0].apply_front(tt_inp_out);
    tested_gates[1].apply_back(tt_inp_out);

    tested_gates[1].apply_front(tt_inp_inp);
    tested_gates[0].apply_front(tt_inp_inp);

    REQUIRE(tt_out_out == tt_out_inp);
    REQUIRE(tt_out_out == tt_inp_out);
    REQUIRE(tt_out_out == tt_inp_inp);
    REQUIRE(tt_out_inp == tt_inp_out);
    REQUIRE(tt_out_inp == tt_inp_inp);
    REQUIRE(tt_inp_out == tt_inp_inp);
  }

  SECTION("many gates") {
    const auto gates_num = mrnd() % 16UL + 1UL;
    std::vector<gate> tested_gates;
    tested_gates.reserve(gates_num);
    for (auto i = 0UL; i < gates_num; i++) {
      tested_gates.emplace_back(bits, mrnd);
    }

    auto tt_forward = truth_table(bits);
    auto tt_backward = truth_table(bits);

    for (auto gate : tested_gates) {
      gate.apply_back(tt_forward);
    }

    for (auto gate : tested_gates | std::views::reverse) {
      gate.apply_front(tt_backward);
    }
    REQUIRE(tt_forward == tt_backward);
  }
}
