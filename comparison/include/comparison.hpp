#pragma once
#include "my_utility.hpp"
#include <chrono>
#include <format>
#include <set>
#include <utility>
#include <vector>

namespace Labs {

using size_t = std::size_t;

template <typename KeyT> struct comp_results final {
    KeyT dev_;
    double set_time_;
    double RBT_time_;
    KeyT mid_range_;

    comp_results(KeyT dev, double set_time, double RBT_time, KeyT mid) noexcept
        : dev_(dev), set_time_(set_time), RBT_time_(RBT_time), mid_range_(mid) {}
}; // namespace template<typenameKeyT>structcomp_results final

// clang-format off
template <typename KeyT> std::ostream &operator<<(std::ostream &os, const comp_results<KeyT> &obj) {
    KeyT left_bnd = obj.mid_range_ - obj.dev_;
    KeyT right_bnd = obj.mid_range_ + obj.dev_;
    os << std::format(R"(For range [{}, {}]:
average time for std::set range query: {:.3f} ns
average time for RBT range query:      {:.3f} ns
    )", left_bnd, right_bnd, obj.set_time_, obj.RBT_time_);
    return os;
}
// clang-format on

template <typename KeyT> class Benchmarker final {
    std::vector<KeyT> middle_dev_;
    KeyT mid_range_;
    size_t warmups_;
    size_t rep_count_;
    std::vector<comp_results<KeyT>> log_;

    // Warms up the cache before the control time measurement
    template <typename TreeType> void warmup(const TreeType& tree, KeyT dev) const {
        KeyT fst = mid_range_ - dev;
        KeyT snd = mid_range_ + dev;
        for (size_t i = 0; i < warmups_; ++i) {
            volatile size_t count = range_query(tree, fst, snd);
        }
    }

    // Measures the average time for a particular tree for a particular range
    template <typename TreeType>
    double measure_performance(const TreeType& tree, const KeyT dev) const {
        warmup(tree, dev);
        KeyT fst = mid_range_ - dev;
        KeyT snd = mid_range_ + dev;

        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < rep_count_; ++i) {
            volatile size_t res = range_query(tree, fst, snd);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> total_time = end - start;
        return total_time.count() / rep_count_;
    }

    // Measures the average time for two trees .First - std::set, second - my tree
    template <typename TreeType1, typename TreeType2>
    std::pair<double, double> pair_measure_performance(const TreeType1& tree1,
                                                       const TreeType2& tree2, KeyT dev) const {
        double avg_set_time = measure_performance<TreeType1>(tree1, dev);
        double avg_RBT_time = measure_performance<TreeType2>(tree2, dev);
        return {avg_set_time, avg_RBT_time};
    }

  public:
    // Fills the resulting std::vector with the measurement results
    Benchmarker(const std::vector<KeyT>& mid_dev, const KeyT& mid, const size_t warmups = 100,
                const size_t rep_count = 300)
        : middle_dev_(mid_dev), mid_range_(mid), warmups_(warmups), rep_count_(rep_count) {}

    template <typename TreeType1, typename TreeType2>
    void run_benchmark(const TreeType1& set, const TreeType2& RBT) {
        for (auto&& elem : middle_dev_) {
            auto pair = pair_measure_performance(set, RBT, elem);
            log_.emplace_back(elem, pair.first, pair.second, mid_range_);
        }
    }

    auto begin() const noexcept {
        return log_.begin();
    }
    auto end() const noexcept {
        return log_.end();
    }

    auto cbegin() const noexcept {
        return log_.cbegin();
    }
    auto cend() const noexcept {
        return log_.cend();
    }

}; // <-- class efficiency_comparison

}; // namespace Labs