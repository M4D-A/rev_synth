#pragma once

#include "circuit.hpp"
#include <random>

uint64_t assess(const std::vector<uint8_t> &lhs,
                const std::vector<uint8_t> &rhs);

std::vector<uint64_t> random_unique_vector(uint64_t vector_size,
                                           uint64_t range_upper,
                                           std::mt19937_64 mrnd);

circuit anneal(const uint64_t size, const std::vector<uint8_t> &target_tt,
               uint64_t seed);
