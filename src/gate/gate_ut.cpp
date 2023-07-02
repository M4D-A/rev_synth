#include "gate.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>

namespace gate_ut {
const auto EPOCHS = 10000;
const auto max_bits = 12;
std::mt19937_64 mrnd;
// const uint64_t mask64 = std::numeric_limits<uint64_t>::max();

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

  REQUIRE(tested.get_size() == bits);
  REQUIRE(tested.get_controls() == controls);

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
