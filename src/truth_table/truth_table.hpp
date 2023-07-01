#include <cstdint>
#include <iostream>
#include <vector>

class truth_table {
private:
  const uint64_t bits_num;
  const uint64_t bits_mask;
  std::vector<uint64_t> data;

public:
  truth_table(uint64_t bits);

  auto get_bits_num() const -> uint64_t;
  auto get_bits_mask() const -> uint64_t;
  auto get_data() const -> std::vector<uint64_t>;
  auto get_row(uint64_t index) const -> uint64_t;

  auto set_data(std::vector<uint64_t>) -> void;
  auto set_row(uint64_t index, uint64_t value) -> void;

  auto print() const -> void;
};
