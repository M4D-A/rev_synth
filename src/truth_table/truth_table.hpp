#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

class truth_table {
private:
  const uint64_t bits_num_;
  const uint64_t bits_mask_;
  std::vector<uint64_t> data_;

public:
  truth_table(uint64_t bits);

  auto get_bits_num() const -> uint64_t;
  auto get_size() const -> uint64_t;

  auto get_bits_mask() const -> uint64_t;
  auto get_data() const -> std::vector<uint64_t>;
  auto get_row(uint64_t index) const -> uint64_t;

  auto set_data(std::vector<uint64_t> data) -> void;
  auto set_row(uint64_t index, uint64_t value) -> void;
  auto inverse() -> void;
  auto shuffle(std::mt19937_64 &mrnd) -> void;
  auto swap(uint64_t index_1, uint64_t index_2) -> void;

  auto operator[](uint64_t index) -> uint64_t &;
  auto operator[](uint64_t index) const -> uint64_t;

  auto operator==(const truth_table &rhs) const -> bool = default;
  auto operator+(const truth_table &rhs) const -> truth_table;

  auto print() const -> void;
};
