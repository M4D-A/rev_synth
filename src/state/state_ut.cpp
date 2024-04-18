#include "state.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>

namespace state_ut {
const auto EPOCHS = 10000;
} // namespace state_ut
using namespace state_ut;

TEST_CASE("state constructors and getters", "[state], [ctors], [getters]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1UL, state::MAX_SIZE))));

  SECTION("assigned value") {
    const auto mask = state::mask(size);
    const auto value = mrnd() & mask;
    auto tested = state(size, value);

    REQUIRE(tested.size() == size);
    REQUIRE(tested.mask() == state::mask(size));
    REQUIRE(tested.value() == value);
    REQUIRE(tested == value);
    for (auto i = 0UL; i < tested.size(); i++) {
      auto rhs = (value >> i) & 1UL;
      REQUIRE(tested.bit_value(i) == rhs);
      REQUIRE(tested[i] == rhs);
    }
    for (auto i = tested.size(); i < state::MAX_SIZE; i++) {
      REQUIRE(tested.bit_value(i) == 0UL);
      REQUIRE(tested[i] == 0UL);
    }
    auto zeroes = tested.zeroes();
    auto ones = tested.ones();
    for (auto zero : zeroes) {
      REQUIRE(tested.bit_value(zero) == 0);
    }
    for (auto one : ones) {
      REQUIRE(tested.bit_value(one) == 1);
    }
  }
}

TEST_CASE("state value setters", "[state], [setters]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1UL, state::MAX_SIZE))));

  const auto mask = state::mask(size);
  const auto init_value = mrnd() & mask;
  const auto chained_value = mrnd() & mask;
  const auto new_value = mrnd() & mask;
  auto tested = state(size, init_value).set_value(chained_value);

  REQUIRE(tested.value() == chained_value);
  REQUIRE(tested == chained_value);

  tested.set_value(new_value);

  REQUIRE(tested.size() == size);
  REQUIRE(tested.mask() == mask);
  REQUIRE(tested.value() == new_value);
  REQUIRE(tested == new_value);
}

TEST_CASE("state bit setters", "[state], [setters]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1UL, state::MAX_SIZE))));

  const auto mask = state::mask(size);
  const auto value = mrnd() & mask;
  const auto new_bit = mrnd() % 2;
  const auto new_bit_index = mrnd() % size;
  auto tested = state(size, value).set_bit(new_bit_index, new_bit);

  REQUIRE(tested.size() == size);
  REQUIRE(tested.mask() == mask);
  for (auto i = 0UL; i < tested.size(); i++) {
    auto rhs = (value >> i) & 1UL;
    if (i == new_bit_index) {
      rhs = new_bit;
    }
    REQUIRE(tested[i] == rhs);
  }
  for (auto i = tested.size(); i < state::MAX_SIZE; i++) {
    REQUIRE(tested[i] == 0);
  }
}

TEST_CASE("state bit switch", "[state], [modifiers]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1UL, state::MAX_SIZE))));

  const auto mask = state::mask(size);
  const auto value = mrnd() & mask;
  const auto new_bit_index = mrnd() % size;
  auto tested = state(size, value).switch_bit(new_bit_index);

  REQUIRE(tested.size() == size);
  REQUIRE(tested.mask() == mask);
  for (auto i = 0UL; i < tested.size(); i++) {
    auto rhs = (value >> i) & 1UL;
    if (i == new_bit_index) {
      rhs = (~rhs) & 1UL;
    }
    REQUIRE(tested[i] == rhs);
  }
  for (auto i = tested.size(); i < state::MAX_SIZE; i++) {
    REQUIRE(tested[i] == 0);
  }
}

TEST_CASE("state negate", "[state], [modifiers]") {
  std::mt19937_64 mrnd;
  const auto size = static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1UL, state::MAX_SIZE))));

  const auto mask = state::mask(size);
  const auto value = mrnd() & mask;
  auto tested = state(size, value).negate();

  REQUIRE(tested.size() == size);
  REQUIRE(tested.mask() == mask);
  for (auto i = 0UL; i < tested.size(); i++) {
    auto rhs = (~value >> i) & 1UL;
    REQUIRE(tested[i] == rhs);
  }

  for (auto i = tested.size(); i < state::MAX_SIZE; i++) {
    REQUIRE(tested[i] == 0);
  }
}
