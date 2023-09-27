#include "circuit/circuit.hpp"
#include "truth_table/truth_table.hpp"

class synthesiser {
public:
  virtual auto synthesize(truth_table target_tt) const -> circuit = 0;
};
