#include <cstdint>
#include <random>

auto random_unique_vector(uint64_t vector_size, uint64_t range_upper, std::mt19937_64 mrnd)
    -> std::vector<uint64_t>;
