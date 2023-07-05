#include "state.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>

namespace state_ut {
const auto EPOCHS = 10000;
std::mt19937_64 mrnd;
const uint64_t mask64 = std::numeric_limits<uint64_t>::max();
} // namespace state_ut

using namespace state_ut;

TEST_CASE("state constructors and getters", "[state], [ctors], [getters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  SECTION("default value") {
    auto tested = state(bits);

    REQUIRE(tested.get_bits_num() == bits);
    REQUIRE(tested.get_bits_mask() == mask64 >> (64 - bits));
    REQUIRE(tested.get_value() == 0UL);
    REQUIRE(tested == 0UL);
    for (auto i = 0UL; i < 64UL; i++) {
      REQUIRE(tested.get_bit(i) == 0UL);
      REQUIRE(tested[i] == 0UL);
    }
  }

  SECTION("assigned value") {
    const auto max_value_mask = mask64 >> (64 - bits);
    const auto value = mrnd() & max_value_mask;
    auto tested = state(bits, value);

    REQUIRE(tested.get_bits_num() == bits);
    REQUIRE(tested.get_bits_mask() == mask64 >> (64 - bits));
    REQUIRE(tested.get_value() == value);
    REQUIRE(tested == value);
    for (auto i = 0UL; i < tested.get_bits_num(); i++) {
      auto rhs = (value >> i) & 1;
      REQUIRE(tested.get_bit(i) == rhs);
      REQUIRE(tested[i] == rhs);
    }
    for (auto i = tested.get_bits_num(); i < 64; i++) {
      REQUIRE(tested.get_bit(i) == 0UL);
      REQUIRE(tested[i] == 0UL);
    }
  }
}

TEST_CASE("state value setters", "[state], [setters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  const auto max_value_mask = mask64 >> (64 - bits);
  const auto value = mrnd() & max_value_mask;
  auto tested = state(bits, value);
  const auto new_value = mrnd() & max_value_mask;
  tested.set_value(new_value);

  REQUIRE(tested.get_bits_num() == bits);
  REQUIRE(tested.get_bits_mask() == max_value_mask);
  REQUIRE(tested.get_value() == new_value);
  REQUIRE(tested == new_value);
}

TEST_CASE("state bit setters", "[state], [setters]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  const auto max_value_mask = mask64 >> (64 - bits);
  const auto value = mrnd() & max_value_mask;
  auto tested = state(bits, value);
  const auto new_bit = mrnd() % 2;
  const auto new_bit_index = mrnd() % bits;
  tested.set_bit(new_bit_index, new_bit);

  REQUIRE(tested.get_bits_num() == bits);
  REQUIRE(tested.get_bits_mask() == max_value_mask);
  for (auto i = 0UL; i < tested.get_bits_num(); i++) {
    auto rhs = (value >> i) & 1;
    if (i == new_bit_index) {
      rhs = new_bit;
    }
    REQUIRE(tested[i] == rhs);
  }
  for (auto i = tested.get_bits_num(); i < 64; i++) {
    REQUIRE(tested[i] == 0);
  }
}

TEST_CASE("state bit switch", "[state], [modifiers]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  const auto max_value_mask = mask64 >> (64 - bits);
  const auto value = mrnd() & max_value_mask;
  auto tested = state(bits, value);
  const auto new_bit_index = mrnd() % bits;
  tested.switch_bit(new_bit_index);

  REQUIRE(tested.get_bits_num() == bits);
  REQUIRE(tested.get_bits_mask() == max_value_mask);
  for (auto i = 0UL; i < tested.get_bits_num(); i++) {
    auto rhs = (value >> i) & 1;
    if (i == new_bit_index) {
      rhs = (~rhs) & 1;
    }
    REQUIRE(tested[i] == rhs);
  }
  for (auto i = tested.get_bits_num(); i < 64; i++) {
    REQUIRE(tested[i] == 0);
  }
}

TEST_CASE("state negate", "[state], [modifiers]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  const auto max_value_mask = mask64 >> (64 - bits);
  const auto value = mrnd() & max_value_mask;
  auto tested = state(bits, value);
  tested.negate();

  REQUIRE(tested.get_bits_num() == bits);
  REQUIRE(tested.get_bits_mask() == max_value_mask);
  for (auto i = 0UL; i < tested.get_bits_num(); i++) {
    auto rhs = (~value >> i) & 1;
    REQUIRE(tested[i] == rhs);
  }

  for (auto i = tested.get_bits_num(); i < 64; i++) {
    REQUIRE(tested[i] == 0);
  }
}
