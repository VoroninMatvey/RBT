#include "RBT.hpp"
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

template <typename C>
std::size_t my_distance(const C &tree, typename C::iterator start, typename C::iterator fin) {
    std::size_t num_start = tree.rank(start);
    std::size_t num_fin = tree.rank(fin);
    return (num_fin > num_start ? (num_fin - num_start) : 0);
}

template <typename T, typename C> std::size_t range_query(const C &tree, T fst, T snd) {
    auto start = tree.lower_bound(fst);
    auto fin = tree.upper_bound(snd);
    return my_distance(tree, start, fin);
}

template <typename KeyT, typename Comparator = std::less<KeyT>>
void requests_reading(std::vector<size_t> &vec) {
    details::Red_Black_Tree<KeyT, Comparator> tree{};
    char request;

    while (std::cin >> request) {
        switch (request) {

        case 'k': {
            KeyT key;
            std::cin >> key;
            tree.insert(key);
            break;
        }

        case 'q': {
            KeyT left_bnd, right_bnd;
            std::cin >> left_bnd >> right_bnd;
            std::size_t range = range_query(tree, left_bnd, right_bnd);
            vec.push_back(range);
            break;
        }

        default:
            throw std::runtime_error("Unknown request: " + std::string{request});
        }
    }
}

template <typename KeyT> void print_range_list(const std::vector<KeyT> &vec) {
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<KeyT>(std::cout, " "));
    std::cout << std::endl;
}
} // namespace

int main() {

    std::vector<size_t> range_list;
    requests_reading<int>(range_list);
    print_range_list(range_list);

    return 0;
}