#pragma once

#include "Node.h"

#include <iostream>
#include <memory>
#include <algorithm>

namespace avl {

template <class K, class V = void>
class Tree {
    private:
        using NodeType = typename avl::Node<K, V>;
    public:
        using ValueReturnType = typename NodeType::ValueReturnType;
        template <class ...Value>
        void insert(const K &key, Value... value) {
            if (!root_) {
                root_ = NodeType::create(key, value...);
            } else {
                bool result;
                std::tie(root_, result) = root_->insert(std::move(root_), key, value...);
            }
        }

        std::optional<ValueReturnType> remove(const K &key) {
            std::optional<ValueReturnType> data;
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
        typename NodeType::NodePtr root_;
};

template <class T> using Set = Tree<T>;
template <class K, class V> using Map = Tree<K, V>;

}
