#include "circuit.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <random>

namespace circuit_ut {
const auto EPOCHS = 10000;
const auto max_bits = 12;
std::mt19937_64 mrnd;
} // namespace circuit_ut

using namespace circuit_ut;

TEST_CASE("circuit constructors and getters", "[circuit], [ctors], [getters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  auto tested = circuit(bits);
  REQUIRE(tested.gates_num() == 0UL);
  REQUIRE(tested.gates() == std::deque<gate>());
  REQUIRE(tested.bits_num() == bits);
  REQUIRE(tested.output_tt() == truth_table(bits));
}

TEST_CASE("circuit extensions and applications", "[circuit], [push]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  const auto gates_num = 1 + mrnd() % 32;
  auto tested = circuit(bits);

  auto tt = truth_table(bits);

  for (auto i = 0UL; i < gates_num; i++) {
    auto random_gate = gate(bits, mrnd);

    random_gate.apply_back(tt);
    tested.push_back(random_gate);
  }
  REQUIRE(tested.gates_num() == gates_num);
  REQUIRE(tested.bits_num() == bits);
  REQUIRE(tested.output_tt() == tt);

  for (auto i = 0UL; i < gates_num; i++) {
    auto random_gate = gate(bits, mrnd);
    random_gate.apply_front(tt);
    tested.push_front(random_gate);
  }

  REQUIRE(tested.gates_num() == gates_num * 2);
  REQUIRE(tested.bits_num() == bits);
  REQUIRE(tested.output_tt() == tt);

  auto random_tt_f = truth_table(bits).shuffle(mrnd);
  auto random_tt_f_copy = random_tt_f;
  tested.apply_front(random_tt_f);
  REQUIRE(random_tt_f == tested.output_tt() + random_tt_f_copy);

  auto random_tt_b = truth_table(bits).shuffle(mrnd);
  auto random_tt_b_copy = random_tt_b;
  tested.apply_back(random_tt_b);
  REQUIRE(random_tt_b == random_tt_b_copy + tested.output_tt());
}

TEST_CASE("circuit operators", "[circuit], [operators]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  SECTION("+= operator") {
    const auto gates_num = 1 + mrnd() % 32;
    auto tested = circuit(bits);
    auto tt = truth_table(bits);

    for (auto i = 0UL; i < gates_num; i++) {
      auto random_gate = gate(bits, mrnd);

      random_gate.apply_back(tt);
      tested += random_gate;
    }
    REQUIRE(tested.gates_num() == gates_num);
    REQUIRE(tested.bits_num() == bits);
    REQUIRE(tested.output_tt() == tt);
  }

  SECTION("+ operator") {
    auto tested_1 = circuit(bits, 1 + mrnd() % 32, mrnd);
    auto tested_2 = circuit(bits, 1 + mrnd() % 32, mrnd);
    auto tested_3 = circuit(bits, 1 + mrnd() % 32, mrnd);

    auto tested_1_copy = tested_1;
    auto tested_2_copy = tested_2;
    auto tested_3_copy = tested_3;

    auto result_12 = tested_1 + tested_2;
    auto result_12_3 = (tested_1 + tested_2) + tested_3;
    auto result_1_23 = tested_1 + (tested_2 + tested_3);

    REQUIRE(tested_1 == tested_1_copy);
    REQUIRE(tested_2 == tested_2_copy);
    REQUIRE(tested_3 == tested_3_copy);
    REQUIRE(result_12_3 == result_1_23);

    auto tt_12 = truth_table(bits);
    auto tt_1_2 = truth_table(bits);

    tested_1.apply_back(tt_1_2);
    tested_2.apply_back(tt_1_2);
    result_12.apply_back(tt_12);

    REQUIRE(tt_1_2 == tt_12);
  }
}
