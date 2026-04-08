#pragma once
#include <iterator>

namespace details {

template <typename C>
std::size_t my_distance(const C &tree, typename C::iterator start, typename C::iterator fin) {
    std::size_t num_start = tree.rank(start);
    std::size_t num_fin = tree.rank(fin);
    return (num_fin > num_start ? (num_fin - num_start) : 0);
}

template <typename T, typename C> std::size_t my_range_query(const C &tree, T fst, T snd) {
    auto start = tree.lower_bound(fst);
    auto fin = tree.upper_bound(snd);
    return my_distance(tree, start, fin);
}

template <typename T, typename C> std::size_t range_query(const C &tree, T fst, T snd) {
    auto start = tree.lower_bound(fst);
    auto fin = tree.upper_bound(snd);
    return std::distance(start, fin);
}

} // namespace details