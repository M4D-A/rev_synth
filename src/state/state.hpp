#include <cstdint>
#include <iostream>

class state {
private:
  const uint64_t bits_num;
  const uint64_t bits_mask;
  uint64_t value;

public:
  state(uint64_t bits, uint64_t value = 0UL);

  auto get_bits_num() const -> uint64_t;
  auto get_bits_mask() const -> uint64_t;
  auto get_value() const -> uint64_t;
  auto get_bit(uint64_t index) const -> uint64_t;

  void set_value(uint64_t value);
  void set_bit(uint64_t index, uint64_t bit_value);
  void switch_bit(uint64_t index);
  void negate();

  void print() const;
};
