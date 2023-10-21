#pragma once
#include "circuit/circuit.hpp"
#include "synthesisers/synthesiser.hpp"

class mmd03 : public synthesiser {
  enum class synth_mode { naive, reduce_cl };
  synth_mode sm_;

public:
  mmd03(synth_mode sm = synth_mode::naive);

  auto synthesize(truth_table target_tt) const -> circuit;
};
