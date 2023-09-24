#include "mmd03.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>
#include <ranges>

namespace mmd03_ut {
const auto EPOCHS = 100;
const auto max_bits = 12;
std::mt19937_64 mrnd;
} // namespace mmd03_ut

using namespace mmd03_ut;

TEST_CASE("mmd03", "[mmd03]") {
  auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  auto target_tt = truth_table(bits);
  target_tt.shuffle(mrnd);
  auto tested = mmd03();
  auto synth = tested.synthesize(target_tt);
  REQUIRE(synth.output_tt() == target_tt);
}
