#include "RBT.hpp"
#include "my_utility.hpp"
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

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
            std::size_t range = details::my_range_query(tree, left_bnd, right_bnd);
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