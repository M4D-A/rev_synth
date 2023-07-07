#include "circuit.hpp"

circuit::circuit(uint64_t width) : output_(truth_table(width)), width_(width){};

auto circuit::push_back(gate new_gate) -> void {
  gates_.push_back(new_gate);
  new_gate.apply_back(output_);
}

auto circuit::push_front(gate new_gate) -> void {
  gates_.push_front(new_gate);
  new_gate.apply_front(output_);
}
