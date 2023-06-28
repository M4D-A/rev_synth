#include "state.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>

const auto EPOCHS = 10000;
std::mt19937_64 mrnd;

TEST_CASE("state constructors and getters", "[ctors], [getters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  SECTION("default value") {
    auto tested = state(bits);

    REQUIRE(tested.get_bits_num() == bits);
    REQUIRE(tested.get_value() == 0UL);
    for (auto i = 0UL; i < tested.get_bits_num(); i++) {
      auto lhs = tested.get_bit(i);
      REQUIRE(lhs == 0);
    }
    for (auto i = tested.get_bits_num(); i < 64; i++) {
      auto lhs = tested.get_bit(i);
      REQUIRE(lhs == 0);
    }
  }

  SECTION("assigned value") {
    const auto max_value_mask = (1UL << bits) - 1UL;
    const auto value = mrnd() & max_value_mask;
    auto tested = state(bits, value);

    REQUIRE(tested.get_bits_num() == bits);
    REQUIRE(tested.get_value() == value);
    for (auto i = 0UL; i < tested.get_bits_num(); i++) {
      auto lhs = tested.get_bit(i);
      auto rhs = (value >> i) & 1;
      REQUIRE(lhs == rhs);
    }
    for (auto i = tested.get_bits_num(); i < 64; i++) {
      auto lhs = tested.get_bit(i);
      REQUIRE(lhs == 0);
    }
  }
}

TEST_CASE("state value setters", "[setters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  SECTION("assigned value") {
    const auto max_value_mask = (1UL << bits) - 1UL;
    const auto value = mrnd() & max_value_mask;
    auto tested = state(bits, value);
    const auto new_value = mrnd() & max_value_mask;
    tested.set_value(new_value);

    REQUIRE(tested.get_bits_num() == bits);
    REQUIRE(tested.get_value() == new_value);
  }
}

TEST_CASE("state bit setters", "[setters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  SECTION("assigned value") {
    const auto max_value_mask = (1UL << bits) - 1UL;
    const auto value = mrnd() & max_value_mask;
    auto tested = state(bits, value);
    const auto new_bit = mrnd() % 2;
    const auto new_bit_index = mrnd() % bits;
    tested.set_bit(new_bit_index, new_bit);

    REQUIRE(tested.get_bits_num() == bits);
    for (auto i = 0UL; i < tested.get_bits_num(); i++) {
      auto lhs = tested.get_bit(i);
      auto rhs = (value >> i) & 1;
      if (i == new_bit_index) {
        rhs = new_bit;
      }
      REQUIRE(lhs == rhs);
    }
    for (auto i = tested.get_bits_num(); i < 64; i++) {
      auto lhs = tested.get_bit(i);
      REQUIRE(lhs == 0);
    }
  }
}

TEST_CASE("state bit switch", "[modifiers]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  const auto max_value_mask = (1UL << bits) - 1UL;
  const auto value = mrnd() & max_value_mask;
  auto tested = state(bits, value);
  const auto new_bit_index = mrnd() % bits;
  tested.switch_bit(new_bit_index);

  REQUIRE(tested.get_bits_num() == bits);
  for (auto i = 0UL; i < tested.get_bits_num(); i++) {
    auto lhs = tested.get_bit(i);
    auto rhs = (value >> i) & 1;
    if (i == new_bit_index) {
      rhs = (~rhs) & 1;
    }
    REQUIRE(lhs == rhs);
  }
  for (auto i = tested.get_bits_num(); i < 64; i++) {
    auto lhs = tested.get_bit(i);
    REQUIRE(lhs == 0);
  }
}

TEST_CASE("state negate", "[modifiers]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  const auto max_value_mask = (1UL << bits) - 1UL;
  const auto value = mrnd() & max_value_mask;
  auto tested = state(bits, value);
  tested.negate();

  REQUIRE(tested.get_bits_num() == bits);
  for (auto i = 0UL; i < tested.get_bits_num(); i++) {
    auto lhs = tested.get_bit(i);
    auto rhs = (~value >> i) & 1;
    REQUIRE(lhs == rhs);
  }

  for (auto i = tested.get_bits_num(); i < 64; i++) {
    auto lhs = tested.get_bit(i);
    REQUIRE(lhs == 0);
  }
}
