#pragma once

#include "circuit.hpp"
#include <random>

circuit anneal(const uint64_t size, const std::vector<uint8_t> &target_tt,
               uint64_t seed);
