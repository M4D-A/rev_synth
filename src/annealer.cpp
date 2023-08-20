#include "annealer.hpp"

uint64_t assess(const std::vector<uint8_t> &lhs,
                const std::vector<uint8_t> &rhs) {
  uint64_t size = std::min(lhs.size(), rhs.size());
  uint64_t matchings = 0;
  for (uint64_t i = 0; i < size; i++) {
    if (lhs[i] == rhs[i]) {
      matchings++;
    }
  }
  return matchings;
}

std::vector<uint64_t> random_unique_vector(uint64_t vector_size,
                                           uint64_t range_upper,
                                           std::mt19937_64 mrnd) {

  std::vector<uint64_t> numbers(range_upper);
  std::iota(numbers.begin(), numbers.end(), 0);
  std::shuffle(numbers.begin(), numbers.end(), mrnd);
  std::vector<uint64_t> result(vector_size);
  std::copy(numbers.begin(),
            numbers.begin() + static_cast<ptrdiff_t>(vector_size),
            result.begin()); // Copy the first k numbers

  return result;
}
