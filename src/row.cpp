#include "row.hpp"
#include "iostream"
#include <cassert>

row::row(uint64_t bits_, uint64_t value_) {
  assert(bits >= 1 && bits <= 64);
  bits = bits_;
  auto mask = (1ULL << bits) - 1ULL;
  value = value_ & mask;
}

void row::print() {
  for (auto i = bits; i > 0; i--) {
    auto bit = (value >> (i - 1)) & 1;
    std::cout << bit;
  }
  std::cout << std::endl;
}
