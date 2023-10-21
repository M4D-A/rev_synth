#include "mmd03/mmd03.hpp"
#include <algorithm>
#include <catch2/catch_all.hpp>
#include <catch2/catch_message.hpp>
#include <random>
#include <ranges>

namespace synth_benchmark_ut {
// const auto EPOCHS = 100;
// const auto max_bits = 12;
std::mt19937_64 mrnd;

auto benchmark_full_3bit(const synthesiser &tested)
    -> std::pair<double, double> {
  auto bits = 3UL;
  auto target_tt = truth_table(bits);
  auto gc_histogram = std::vector<uint64_t>(20, 0);
  auto cl_histogram = std::vector<uint64_t>(20, 0);
  auto gc_sum = 0UL;
  auto cl_sum = 0UL;
  do {
    auto circ = tested.synthesize(target_tt);
    auto circ_size = circ.gates_num();
    auto circ_cl = circ.controls_num();
    gc_histogram[circ_size]++;
    cl_histogram[circ_cl]++;
    gc_sum += circ_size;
    cl_sum += circ_cl;
    REQUIRE(circ.output_tt() == target_tt);
  } while (target_tt.next_permutation());

  auto circ_nums =
      std::accumulate(gc_histogram.begin(), gc_histogram.end(), 0.0);
  auto average_gc = static_cast<double>(gc_sum) / circ_nums;
  auto average_cl = static_cast<double>(cl_sum) / circ_nums;

  return {average_gc, average_cl};
}

auto benchmark_sample(const synthesiser &tested,
                      const std::vector<truth_table> &sample)
    -> std::pair<double, double> {
  auto gc_histogram = std::vector<uint64_t>(20, 0);
  auto cl_histogram = std::vector<uint64_t>(20, 0);
  auto gc_sum = 0UL;
  auto cl_sum = 0UL;
  for (auto &&target_tt : sample) {
    auto circ = tested.synthesize(target_tt);
    auto circ_size = circ.gates_num();
    auto circ_cl = circ.controls_num();
    if (circ_size >= gc_histogram.size())
      gc_histogram.resize(circ_size + 1, 0);
    if (circ_cl >= cl_histogram.size())
      cl_histogram.resize(circ_cl + 1, 0);
    gc_histogram[circ_size]++;
    cl_histogram[circ_cl]++;
    gc_sum += circ_size;
    cl_sum += circ_cl;
    REQUIRE(circ.output_tt() == target_tt);
  }

  auto circ_nums =
      std::accumulate(gc_histogram.begin(), gc_histogram.end(), 0.0);
  auto average_gc = static_cast<double>(gc_sum) / circ_nums;
  auto average_cl = static_cast<double>(cl_sum) / circ_nums;
  return {average_gc, average_cl};
}

} // namespace synth_benchmark_ut

using namespace synth_benchmark_ut;

TEST_CASE("Full 3 bit benchmark", "[benchmark]") {
  std::cout << "Full 3 bit BM" << std::endl;
  SECTION("base mmd03") {
    std::cout << " Base MMD03" << std::endl;
    auto tested = mmd03();
    auto [gc, cl] = benchmark_full_3bit(tested);
    std::cout << "  Average GC: " << gc << std::endl;
    std::cout << "  Average CL: " << cl << std::endl;
  }
  std::cout << std::endl;
}

TEST_CASE("Sample 4 bit benchmark", "[benchmark], [sample], [4bit]") {
  std::cout << "Sample 4 bit BM: " << std::endl;
  auto sample = std::vector<truth_table>(100, truth_table(4));
  std::for_each(sample.begin(), sample.end(),
                [](truth_table &tt) { tt.shuffle(mrnd); });

  SECTION("base mmd03") {
    std::cout << " Base MMD03" << std::endl;
    auto tested = mmd03();
    auto [gc, cl] = benchmark_sample(tested, sample);
    std::cout << "  Average GC: " << gc << std::endl;
    std::cout << "  Average CL: " << cl << std::endl;
  }
  std::cout << std::endl;
}
