#include "mmd03.hpp"
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>
#include <ranges>

namespace mmd03_ut {
const auto EPOCHS = 100;
const auto max_bits = 12;
std::mt19937_64 mrnd;
} // namespace mmd03_ut

using namespace mmd03_ut;

TEST_CASE("mmd03", "[mmd03]") {
  auto bits =
      static_cast<uint64_t>(GENERATE(take(EPOCHS, random(1, max_bits))));

  auto target_tt = truth_table(bits);
  target_tt.shuffle(mrnd);
  auto tested = mmd03();
  auto synth = tested.synthesize(target_tt);
  REQUIRE(synth.output_tt() == target_tt);
}

TEST_CASE("paper experimental results for base algorithm", "[mmd03], [paper]") {
  auto bits = 3UL;
  auto target_tt = truth_table(bits);
  auto gc_histogram = std::vector<uint64_t>(20, 0);
  auto cl_histogram = std::vector<uint64_t>(25, 0);
  auto gc_sum = 0UL;
  auto cl_sum = 0UL;
  auto tested = mmd03();
  do {
    auto circ = tested.synthesize(target_tt);
    auto circ_size = circ.gates_num();
    auto cls_num = circ.controls_num();
    gc_histogram[circ_size]++;
    cl_histogram[cls_num]++;
    gc_sum += circ_size;
    cl_sum += cls_num;
  } while (target_tt.next_permutation());

  auto circ_nums =
      std::accumulate(gc_histogram.begin(), gc_histogram.end(), 0.0);
  auto average_gc = static_cast<double>(gc_sum) / circ_nums;
  auto average_cl = static_cast<double>(cl_sum) / circ_nums;

  for (auto i = 0UL; i < gc_histogram.size(); i++) {
    std::cout << i << ": " << gc_histogram[i] << std::endl;
  }
  std::cout << "AV GC: " << average_gc << std::endl << std::endl;
  for (auto i = 0UL; i < cl_histogram.size(); i++) {
    std::cout << i << ": " << cl_histogram[i] << std::endl;
  }
  std::cout << "AV CL: " << average_cl << std::endl << std::endl;
}
