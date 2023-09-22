#include "circuit.hpp"

circuit::circuit(uint64_t bits_num)
    : output_tt_(truth_table(bits_num)), bits_num_(bits_num){};

circuit::circuit(uint64_t bits_num, uint64_t gates_num, std::mt19937_64 mrnd)
    : output_tt_(truth_table(bits_num)), bits_num_(bits_num) {
  for (auto i = 0UL; i < gates_num; i++) {
    push_back(gate(bits_num_, mrnd));
  }
}

auto circuit::bits_num() const -> uint64_t { return bits_num_; }

auto circuit::gates_num() const -> uint64_t { return gates_.size(); }

auto circuit::gates() const -> const std::deque<gate> & { return gates_; }

auto circuit::output_tt() const -> const truth_table & { return output_tt_; }

auto circuit::apply(uint64_t row) const -> uint64_t { return output_tt_[row]; }

auto circuit::apply(state &s) const -> void {
  assert(s.bits_num() == bits_num_);
  s.set_value(apply(s.value()));
}

auto circuit::apply_back(truth_table &tt) const -> void {
  assert(tt.bits_num() == bits_num_);
  tt = tt + output_tt_;
}

auto circuit::apply_front(truth_table &tt) const -> void {
  assert(tt.bits_num() == bits_num_);
  tt = output_tt_ + tt;
}

auto circuit::push_back(gate new_gate) -> circuit & {
  assert(new_gate.bits_num() == bits_num_);
  gates_.push_back(new_gate);
  new_gate.apply_back(output_tt_);
  return *this;
}

auto circuit::push_front(gate new_gate) -> circuit & {
  assert(new_gate.bits_num() == bits_num_);
  gates_.push_front(new_gate);
  new_gate.apply_front(output_tt_);
  return *this;
}

auto circuit::push_back(const circuit &new_circuit) -> circuit & {
  for (auto gate : new_circuit.gates()) {
    push_back(gate);
  }
  return *this;
}

auto circuit::push_front(const circuit &new_circuit) -> circuit & {
  for (auto gate : new_circuit.gates()) {
    push_front(gate);
  }
  return *this;
}

auto circuit::operator[](uint64_t index) const -> gate { return gates_[index]; }

template <typename T> auto circuit::operator+=(const T &rhs) -> circuit & {
  push_back(rhs);
  return *this;
}
template auto circuit::operator+=(const gate &rhs) -> circuit &;
template auto circuit::operator+=(const circuit &rhs) -> circuit &;

template <typename T> auto operator+(circuit lhs, const T &rhs) -> circuit {
  lhs += rhs;
  return lhs;
}
template auto operator+(circuit lhs, const gate &rhs) -> circuit;
template auto operator+(circuit lhs, const circuit &rhs) -> circuit;

auto circuit::print() -> void {
  for (auto i = 0UL; i < gates_num(); i++) {
    (*this)[i].print();
  }
}