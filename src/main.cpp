#include "annealer.hpp"
#include "circuit.hpp"
#include <random>

int main() {
  std::vector<uint8_t> tt = {2, 1, 3, 7, 5, 0, 4, 6};
  circuit c = anneal(3, tt, 2137);
  c.print();
}
