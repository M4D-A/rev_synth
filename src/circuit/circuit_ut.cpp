#include "circuit.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
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
  REQUIRE(tested.get_gates_num() == 0UL);
  REQUIRE(tested.get_gates() == std::deque<gate>());
  REQUIRE(tested.get_width() == bits);
  REQUIRE(tested.get_truth_table() == truth_table(bits));
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
  REQUIRE(tested.get_gates_num() == gates_num);
  REQUIRE(tested.get_width() == bits);
  REQUIRE(tested.get_truth_table() == tt);

  for (auto i = 0UL; i < gates_num; i++) {
    auto random_gate = gate(bits, mrnd);
    random_gate.apply_front(tt);
    tested.push_front(random_gate);
  }

  REQUIRE(tested.get_gates_num() == gates_num * 2);
  REQUIRE(tested.get_width() == bits);
  REQUIRE(tested.get_truth_table() == tt);

  auto random_tt_f = truth_table(bits).shuffle(mrnd);
  auto random_tt_f_copy = random_tt_f;
  tested.apply_front(random_tt_f);
  REQUIRE(random_tt_f == tested.get_truth_table() + random_tt_f_copy);

  auto random_tt_b = truth_table(bits).shuffle(mrnd);
  auto random_tt_b_copy = random_tt_b;
  tested.apply_back(random_tt_b);
  REQUIRE(random_tt_b == random_tt_b_copy + tested.get_truth_table());
}
