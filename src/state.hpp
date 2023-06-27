#include <cstdint>
#include <iostream>

class state {
private:
  uint64_t bits = 1UL;
  uint64_t value = 0UL;

public:
  state(uint64_t bits, uint64_t value = 0UL);

  auto get_bits_num() const -> uint64_t;
  auto get() const -> uint64_t;
  auto get_bit(uint64_t index) const -> uint64_t;
  void print() const;

  void set(uint64_t value);
  void set_bit(uint64_t index, uint64_t bit_value);
  void switch_bit(uint64_t index);
};
