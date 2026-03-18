#include "RBT.hpp"
#include "Tree_builder.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    details::Red_Black_Tree<int> tree;
    /* tree.Tree_Insert(100);
     tree.Tree_Insert(50);
     tree.Tree_Insert(200);
     tree.Tree_Insert(35);
     tree.Tree_Insert(75);
     tree.Tree_Insert(60);
     tree.Tree_Insert(150);
     tree.Tree_Insert(300);
     tree.Tree_Insert(400);*/
    for (int i = 200; i < 600; i += 10) {
        tree.insert(i);
    }

    details::Tree_builder<int> tree_builder(tree, argv[1]);
    tree_builder.dump_tree(0);

    std::cout << "success!" << std::endl;

    return 0;
}