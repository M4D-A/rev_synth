#pragma once
#include "gate/gate.hpp"
#include <cassert>
#include <cstdint>
#include <deque>

class circuit {
  std::deque<gate> gates_;
  truth_table output_;
  const uint64_t width_;

public:
  circuit(uint64_t width_);

  auto push_back(gate new_gate) -> void;
  auto push_front(gate new_gate) -> void;
};
