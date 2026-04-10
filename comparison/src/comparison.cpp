#include "comparison.hpp"
#include "RBT.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <set>
#include <vector>

namespace {

template <typename T> void create_shuffle_range(std::vector<T>& vec) {
    std::iota(vec.begin(), vec.end(), T{});

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(vec.begin(), vec.end(), gen);
}

template <typename T>
void print_results(const Labs::Benchmarker<T>& obj, std::ostream& os = std::cout) {
    os << "Time measurements are taken for the range_query method over a tree consisting of "
          "sequential elements from 0 to 1000000"
       << std::endl
       << std::endl;
    std::copy(obj.begin(), obj.end(), std::ostream_iterator<Labs::comp_results<T>>(os, "\n"));
}
} // namespace

int main() {
    std::size_t size = 1000000;
    std::vector<std::size_t> vec(size);
    create_shuffle_range(vec);
    std::set set(vec.begin(), vec.end());
    Labs::Red_Black_Tree my_tree(vec.begin(), vec.end());

    std::vector<std::size_t> dev = {10, 100, 1000, 10000, 100000, 300000, 500000};
    Labs::Benchmarker<std::size_t> benchmark(dev, size / 2);
    benchmark.run_benchmark(set, my_tree);
    print_results(benchmark);
}
