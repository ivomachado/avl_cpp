#include "avl/Tree.h"

#include <iostream>
#include <memory>
#include <algorithm>

int main() {
    avl::Tree<int> tree;

    // tree.insert(5);
    // tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(2);
    tree.insert(3);
    tree.insert(1);
    tree.insert(7);
    tree.insert(4);
    // tree.insert(1);

    tree.print();

    tree.remove(2);

    // tree.insert(4);
    // tree.insert(5);
    // tree.insert(6);
    // tree.insert(7);
    // tree.insert(8);
    // tree.insert(9);
    // tree.insert(10);
    // tree.insert(11);
    // tree.insert(12);


    // tree.insert(1);
    // tree.insert(3);
    // tree.insert(2);
    // tree.insert(6);
    // tree.insert(5);
    // tree.insert(4);

    tree.print();

    return EXIT_SUCCESS;
}
