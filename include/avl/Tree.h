#pragma once

#include "Node.h"

#include <iostream>
#include <memory>
#include <algorithm>

namespace avl {

template <typename K>
class Tree {
    public:
        void insert(const K &key) {
            if (!root_) {
                root_ = Node<K>::create(key);
            } else {
                bool result;
                std::tie(root_, result) = root_->insert(std::move(root_), key);
            }
        }

        std::optional<K> remove(const K &key) {
            std::optional<K> data;
            std::tie(root_, data) = root_->remove(std::move(root_), key);

            return data;
        }

        void print() {
            if (root_) {
                std::cout << "root: ";
                root_->print(0);
            }
        }

    private:
        typename Node<K>::NodePtr root_;
};

}
