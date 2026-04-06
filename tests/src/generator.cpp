#include "../include/generator.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Print absolute path of RBT directory";
        return 1;
    }

    std::size_t test_num, key_num;
    int min_value, max_value;
    std::cin >> test_num >> key_num >> min_value >> max_value;

    Tests::generator<int> gen(argv[1], test_num, key_num, min_value, max_value);
    gen.generate();
}