#pragma once
#include "circuit/circuit.hpp"
#include "synthesisers/synthesiser.hpp"

class mmd03 : public synthesiser {
public:
  auto synthesize(truth_table target_tt) const -> circuit;
};
