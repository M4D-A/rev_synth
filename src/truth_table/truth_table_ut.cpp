#include "truth_table.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>

namespace truth_table_ut {
const auto EPOCHS = 10000;
const auto max_bits = 12;
} // namespace truth_table_ut

using namespace truth_table_ut;

TEST_CASE("truth_table constructors and getters", "[truth_table], [ctors], [getters]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  auto tested = truth_table(size);
  REQUIRE(tested.size() == size);
  REQUIRE(tested.mask() == state::mask(size));
  for (auto i = 0UL; auto row : tested) {
    REQUIRE(row == i++);
  }
}

TEST_CASE("truth_table value setters", "[truth_table], [setters]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));
  const auto max_mask = state::mask(size);
  const auto new_value = mrnd() & max_mask;
  const auto index = mrnd() & max_mask;

  auto tested = truth_table(size).set_row(index, new_value);
  REQUIRE(tested.size() == size);
  REQUIRE(tested.mask() == max_mask);
  for (auto i = 0UL; auto row : tested) {
    if (i == index) {
      REQUIRE(row == new_value);
    }
    else {
      REQUIRE(row == i);
    }
    i++;
  }
}

TEST_CASE("truth_table subscript operator", "[truth_table], [subscriptop]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));
  const auto max_mask = state::mask(size);
  const auto new_value = mrnd() & max_mask;
  const auto index = mrnd() & max_mask;

  auto tested = truth_table(size);
  tested[index] = new_value;
  REQUIRE(tested.size() == size);
  REQUIRE(tested.mask() == max_mask);
  for (auto i = 0UL; i < (1UL << size); i++) {
    if (i == index) {
      REQUIRE(tested[i] == new_value);
    }
    else {
      REQUIRE(tested[i] == i);
    }
  }
}

TEST_CASE("truth table inversion", "[truth_table], [inversion]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));
  auto tested = truth_table(size).shuffle(mrnd);
  auto tested_inv = tested;
  tested_inv.inverse();
  for (auto i = 0UL; i < tested.length(); i++) {
    auto output = tested[i];
    auto input = tested_inv[output];
    REQUIRE(i == input);
  }
  tested_inv.inverse();
  REQUIRE(tested_inv == tested);
}

TEST_CASE("truth table addition", "[truth_table], [addition]") {
  std::mt19937_64 mrnd;
  const auto bits = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  SECTION("adding inverses") {
    auto tested = truth_table(bits).shuffle(mrnd);
    auto tested_inv = tested;
    tested_inv.inverse();
    auto sum_a = tested + tested_inv;
    auto sum_b = tested_inv + tested;
    for (auto i = 0UL; i < sum_a.length(); i++) {
      REQUIRE(i == sum_a[i]);
      REQUIRE(i == sum_b[i]);
    }
  }

  SECTION("sum inversion") {
    auto tested_1 = truth_table(bits).shuffle(mrnd);
    auto tested_2 = truth_table(bits).shuffle(mrnd);
    auto tested_12 = (tested_1 + tested_2).inverse();
    tested_1.inverse();
    tested_2.inverse();
    auto tested_21 = tested_2 + tested_1;
    REQUIRE(tested_12 == tested_21);
  }

  SECTION("random elements") {
    auto tested_1 = truth_table(bits).shuffle(mrnd);
    auto tested_2 = truth_table(bits).shuffle(mrnd);
    auto sum = tested_1 + tested_2;
    for (auto input = 0UL; input < sum.length(); input++) {
      auto sum_output = sum[input];
      auto chained_output = tested_2[tested_1[input]];
      REQUIRE(sum_output == chained_output);
    }
  }
}
