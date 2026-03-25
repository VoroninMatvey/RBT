#include "../include/generator.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Print absolute path of RBT directory";
        return 1;
    }

    Tests::generator<int> gen(argv[1], 10, 100);
    gen.generate();
}