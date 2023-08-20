#include "annealer.hpp"
#include "circuit.hpp"
#include <random>

int main() {
  circuit c(4);
  c.push_back(3, {0, 1});
  c.push_back(2, {0, 1, 3});
  c.push_front(3, {0, 1});
  c.push_front(2, {0, 1, 3});
  c.print();
}
