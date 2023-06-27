#include <cstdint>
#include <iostream>

class row {
private:
  uint64_t bits = 1;
  uint64_t value = 0;

public:
  row(uint64_t bits, uint64_t value = 0ULL);

  void get_bit(uint64_t index);
  void set_bit(uint64_t index, uint64_t value);
  void switch_bit(uint64_t index, uint64_t value);
  void set_value(uint64_t value);

  void print();
};
