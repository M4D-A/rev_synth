#include "state.hpp"
#include <catch2/catch_all.hpp>
#include <random>

const auto EPOCHS = 100;
std::mt19937_64 mrnd;

TEST_CASE("state", "[ctors]") {
  const auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, 64))));

  SECTION("default value") {
    auto tested = state(bits);

    REQUIRE(tested.get_bits_num() == bits);
    REQUIRE(tested.get() == 0UL);
  }

  SECTION("assigned value") {
    const auto max_value_mask = (1UL << bits) - 1UL;
    const auto value = mrnd() & max_value_mask;
    auto tested = state(bits, value);

    REQUIRE(tested.get_bits_num() == bits);
    REQUIRE(tested.get() == value);
  }
}
