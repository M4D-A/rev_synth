#include "state.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("aaaa", "[tag1]") {
  row a{5, 0b101110};
  a.print();
}
