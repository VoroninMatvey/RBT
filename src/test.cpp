#include "RBT.hpp"
#include "Tree_builder.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output_dot_file>" << std::endl;
        return 1;
    }

    details::Red_Black_Tree<int> tree;

    std::vector<int> values(40);
    std::iota(values.begin(), values.end(), 1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(values.begin(), values.end(), g);

    std::cout << "Inserting values: ";
    for (int val : values) {
        tree.insert(val);
        std::cout << val << " ";
    }
    std::cout << "\n";
    std::cout << "rank 23: " << tree.rank<true>(23) << std::endl;

    try {
        details::Tree_builder<int> tree_builder(tree, argv[1]);
        tree_builder.dump_tree(0);
        std::cout << "\nGeneration success! Check your .dot file." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during visualization: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
