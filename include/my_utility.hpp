#pragma once
#include <iterator>
#include <set>
#include <type_traits>

namespace Labs {

namespace details {

template <typename C, typename It> std::size_t my_distance(const C &tree, It start, It fin) {
    std::size_t num_start = tree.rank(start);
    std::size_t num_fin = tree.rank(fin);
    return (num_fin > num_start ? (num_fin - num_start) : 0);
}

} // namespace details

// Returns the number of elements in the range [fst, snd]
template <typename T, typename C> std::size_t range_query(const C &tree, T fst, T snd) {
    auto start = tree.lower_bound(fst);
    auto fin = tree.upper_bound(snd);

    if constexpr (std::is_same_v<std::decay_t<C>, std::set<T>>) {
        return std::distance(start, fin);
    } else {
        return details::my_distance(tree, start, fin);
    }
}

} // namespace Labs