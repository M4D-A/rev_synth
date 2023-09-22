#include "mmd03.hpp"
auto mmd03::synthesize(truth_table target_tt) -> circuit {
  auto width = target_tt.bits_num();
  auto retval = circuit(width);

  // f(0) = 0
  auto r0 = target_tt[0];
  for (auto i = 0UL; i < width; i++) {
    auto bit = (r0 >> i) & 1;
    if (bit) {
      auto not_gate = gate(width, {}, i);
      not_gate.apply_back(target_tt);
      retval.push_front(not_gate);
    }
  }

  return retval;
}
