#include "truth_table.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>

namespace truth_table_ut {
const auto EPOCHS = 10000;
const auto max_bits = 12;
std::mt19937_64 mrnd;
const uint64_t mask64 = std::numeric_limits<uint64_t>::max();
} // namespace truth_table_ut

using namespace truth_table_ut;

TEST_CASE("truth_table constructors and getters",
          "[truth_table], [ctors], [getters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  auto tested = truth_table(bits);
  REQUIRE(tested.get_bits_num() == bits);
  REQUIRE(tested.get_bits_mask() == mask64 >> (64 - bits));
  for (auto i = 0UL; i < (1UL << bits); i++) {
    REQUIRE(tested.get_row(i) == i);
  }
}

TEST_CASE("truth_table value setters", "[truth_table], [setters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));
  const auto max_value_mask = mask64 >> (64 - bits);
  auto tested = truth_table(bits);
  const auto new_value = mrnd() & max_value_mask;
  const auto index = mrnd() & max_value_mask;
  tested.set_row(index, new_value);

  REQUIRE(tested.get_bits_num() == bits);
  REQUIRE(tested.get_bits_mask() == max_value_mask);
  for (auto i = 0UL; i < (1UL << bits); i++) {
    if (i == index) {
      REQUIRE(tested.get_row(i) == new_value);
    }
    else {
      REQUIRE(tested.get_row(i) == i);
    }
  }
}

TEST_CASE("truth_table subscript operator", "[truth_table], [subscriptop]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));
  const auto max_value_mask = mask64 >> (64 - bits);
  auto tested = truth_table(bits);
  const auto new_value = mrnd() & max_value_mask;
  const auto index = mrnd() & max_value_mask;
  tested[index] = new_value;

  REQUIRE(tested.get_bits_num() == bits);
  REQUIRE(tested.get_bits_mask() == max_value_mask);
  for (auto i = 0UL; i < (1UL << bits); i++) {
    if (i == index) {
      REQUIRE(tested[i] == new_value);
    }
    else {
      REQUIRE(tested[i] == i);
    }
  }
}

TEST_CASE("truth table inversion", "[truth_table], [inversion]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));
  auto forward = truth_table(bits);
  forward.shuffle(mrnd);
  auto backward = forward;
  backward.inverse();
  for (auto i = 0UL; i < forward.get_size(); i++) {
    auto output = forward[i];
    auto input = backward[output];
    REQUIRE(i == input);
  }
  backward.inverse();
  REQUIRE(backward == forward);
}

TEST_CASE("truth table addition", "[truth_table], [addition]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  SECTION("adding inverses") {
    auto forward = truth_table(bits);
    forward.shuffle(mrnd);
    auto backward = forward;
    backward.inverse();
    auto sum_fb = forward + backward;
    auto sum_bf = backward + forward;
    for (auto i = 0UL; i < sum_fb.get_size(); i++) {
      REQUIRE(i == sum_fb[i]);
      REQUIRE(i == sum_bf[i]);
    }
  }

  SECTION("sum inversion") {
    auto tt_1 = truth_table(bits);
    auto tt_2 = truth_table(bits);

    tt_1.shuffle(mrnd);
    tt_2.shuffle(mrnd);

    auto tt_12 = tt_1 + tt_2;
    tt_12.inverse();

    tt_1.inverse();
    tt_2.inverse();
    auto tt_21 = tt_2 + tt_1;
    REQUIRE(tt_12 == tt_21);
  }

  SECTION("random elements") {
    auto tt_1 = truth_table(bits);
    auto tt_2 = truth_table(bits);

    tt_1.shuffle(mrnd);
    tt_2.shuffle(mrnd);

    auto sum = tt_1 + tt_2;
    for (auto input = 0UL; input < sum.get_size(); input++) {
      auto sum_output = sum[input];
      auto chained_output = tt_2[tt_1[input]];
      REQUIRE(sum_output == chained_output);
    }
  }
}
