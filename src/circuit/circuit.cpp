#include "circuit.hpp"

circuit::circuit(uint64_t width) : output_(truth_table(width)), width_(width){};

auto circuit::get_width() const -> uint64_t { return width_; }

auto circuit::get_gates_num() const -> uint64_t { return gates_.size(); }

auto circuit::get_gates() const -> const std::deque<gate> & { return gates_; }

auto circuit::get_truth_table() const -> const truth_table & { return output_; }

auto circuit::apply(uint64_t row) const -> uint64_t { return output_[row]; }

auto circuit::apply(state &s) const -> void {
  assert(s.get_bits_num() == width_);
  s.set_value(apply(s.get_value()));
}

auto circuit::apply_back(truth_table &tt) const -> void {
  assert(tt.get_bits_num() == width_);
  tt = tt + output_;
}

auto circuit::apply_front(truth_table &tt) const -> void {
  assert(tt.get_bits_num() == width_);
  tt = output_ + tt;
}

auto circuit::push_back(gate new_gate) -> void {
  gates_.push_back(new_gate);
  new_gate.apply_back(output_);
}

auto circuit::push_front(gate new_gate) -> void {
  gates_.push_front(new_gate);
  new_gate.apply_front(output_);
}
