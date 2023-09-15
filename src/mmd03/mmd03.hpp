#pragma once
#include "circuit/circuit.hpp"
class mmd03 {

public:
  auto synthesize(truth_table target_tt) -> circuit;
};
