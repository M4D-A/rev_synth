#include "utils.hpp"

auto random_unique_vector(uint64_t vector_size, uint64_t range_upper, std::mt19937_64 mrnd)
    -> std::vector<uint64_t> {

  std::vector<uint64_t> numbers(range_upper);
  std::iota(numbers.begin(), numbers.end(), 0);
  std::shuffle(numbers.begin(), numbers.end(), mrnd);
  std::vector<uint64_t> result(vector_size);
  std::copy(numbers.begin(), numbers.begin() + static_cast<std::ptrdiff_t>(vector_size),
            result.begin()); // Copy the first k numbers

  return result;
}
