#include "gate.hpp"
#include <bits/fs_fwd.h>
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>
#include <ranges>

namespace gate_ut {
const auto EPOCHS = 10000;
const auto max_bits = 12;
std::mt19937_64 mrnd;
const uint64_t mask64 = std::numeric_limits<uint64_t>::max();

std::vector<uint64_t> random_unique_vector(uint64_t vector_size,
                                           uint64_t range_upper) {
  std::random_device rd;
  std::mt19937 gen(rd());

  std::vector<uint64_t> numbers(range_upper);
  std::iota(numbers.begin(), numbers.end(), 0);

  std::shuffle(numbers.begin(), numbers.end(), gen); // Shuffle the numbers

  std::vector<uint64_t> result(vector_size);
  std::copy(numbers.begin(),
            numbers.begin() + static_cast<ptrdiff_t>(vector_size),
            result.begin()); // Copy the first k numbers

  return result;
}
} // namespace gate_ut

using namespace gate_ut;

TEST_CASE("gate constructors and getters", "[gate], [ctors], [getters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  const auto controls_num = mrnd() % bits + 1;
  auto controls = random_unique_vector(controls_num, bits);
  const auto target = controls.back();
  controls.pop_back();
  const auto tested = gate(bits, controls, target);
  std::shuffle(controls.begin(), controls.end(), mrnd);
  const auto tested_2 = gate(bits, controls, target);
  std::sort(controls.begin(), controls.end());

  REQUIRE(tested.get_size() == bits);
  REQUIRE(tested.get_controls() == controls);
  REQUIRE(tested == tested_2);

  const auto control_mask = tested.get_control_mask();
  for (auto i = 0UL; i < 64UL; i++) {
    const auto is_control_in_vector =
        std::find(controls.begin(), controls.end(), i) != controls.end();
    const auto is_control_bit_set =
        static_cast<bool>((control_mask >> i) & 1UL);
    REQUIRE(is_control_bit_set == is_control_in_vector);
  }

  const auto target_mask = tested.get_target_mask();
  for (auto i = 0UL; i < 64UL; i++) {
    const auto is_target_bit_set = static_cast<bool>((target_mask >> i) & 1UL);
    const auto is_target_bit = i == target;
    REQUIRE(is_target_bit_set == is_target_bit);
  }
}

TEST_CASE("gate apply", "[gate], [apply]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  const auto controls_num = mrnd() % bits + 1;
  auto controls = random_unique_vector(controls_num, bits);
  const auto target = controls.back();
  controls.pop_back();
  const auto tested = gate(bits, controls, target);

  SECTION("row") {
    const auto row = mrnd() & (mask64 >> (64 - bits));
    const auto new_row = tested.apply(row);
    const auto new_new_row = tested.apply(new_row);
    REQUIRE(new_new_row == row);
    const bool is_control_set =
        std::all_of(controls.begin(), controls.end(),
                    [row](auto i) { return (row >> i) & 1; });
    if (is_control_set) {
      REQUIRE((row ^ new_row) == 1UL << target);
    }
    else {
      REQUIRE(row == new_row);
    }
  }

  SECTION("state") {
    const auto value = mrnd() & (mask64 >> (64 - bits));
    auto target_state = state(bits, value);
    tested.apply(target_state);
    const auto new_value = target_state.get_value();
    REQUIRE(new_value == tested.apply(value));
  }

  SECTION("truth table") {
    auto target_tt = truth_table(bits);
    tested.apply(target_tt);
    for (auto input = 0UL; input < target_tt.get_size(); input++) {
      const auto output = target_tt[input];
      REQUIRE(output == tested.apply(input));
    }
  }

  SECTION("truth table on input") {
    auto target_tt = truth_table(bits);
    tested.apply(target_tt, gate::application_side::input);
    for (auto input = 0UL; input < target_tt.get_size(); input++) {
      const auto output = target_tt[input];
      REQUIRE(output == tested.apply(input));
    }
  }
}

TEST_CASE("complex gate apply", "[gate], [apply]") {

  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  SECTION("two gates") {
    std::vector<gate> tested_gates;
    tested_gates.reserve(2UL);
    for (auto i = 0UL; i < 2UL; i++) {
      const auto controls_num = mrnd() % bits + 1;
      auto controls = random_unique_vector(controls_num, bits);
      const auto target = controls.back();
      controls.pop_back();
      tested_gates.emplace_back(bits, controls, target);
    }

    auto tt_out_out = truth_table(bits);
    auto tt_out_inp = truth_table(bits);
    auto tt_inp_out = truth_table(bits);
    auto tt_inp_inp = truth_table(bits);

    tested_gates[0].apply(tt_out_out, gate::application_side::output);
    tested_gates[1].apply(tt_out_out, gate::application_side::output);

    tested_gates[1].apply(tt_out_inp, gate::application_side::output);
    tested_gates[0].apply(tt_out_inp, gate::application_side::input);

    tested_gates[0].apply(tt_inp_out, gate::application_side::input);
    tested_gates[1].apply(tt_inp_out, gate::application_side::output);

    tested_gates[1].apply(tt_inp_inp, gate::application_side::input);
    tested_gates[0].apply(tt_inp_inp, gate::application_side::input);

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
      const auto controls_num = mrnd() % bits + 1;
      auto controls = random_unique_vector(controls_num, bits);
      const auto target = controls.back();
      controls.pop_back();
      tested_gates.emplace_back(bits, controls, target);
    }

    auto tt_forward = truth_table(bits);
    auto tt_backward = truth_table(bits);

    for (auto gate : tested_gates) {
      gate.apply(tt_forward, gate::application_side::output);
    }

    for (auto gate : tested_gates | std::views::reverse) {
      gate.apply(tt_backward, gate::application_side::input);
    }
    REQUIRE(tt_forward == tt_backward);
  }
}
