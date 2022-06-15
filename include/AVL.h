#pragma once

#include "Node.h"

#include <iostream>
#include <memory>
#include <algorithm>

template <typename T>
class AVL {
    public:
        void insert(T &&value) {
            if (!root_) {
                root_ = Node<T>::create(std::move(value));
            } else {
                bool result;
                std::tie(root_, result) = root_->insert(std::move(root_), std::move(value));
            }
        }

        std::optional<T> remove(const T &value) {
            std::optional<T> data;
            std::tie(root_, data) = root_->remove(std::move(root_), value);

            return data;
        }

        void print() {
            if (root_) {
                std::cout << "root: ";
                root_->print(0);
            }
        }

    private:
        typename Node<T>::NodePtr root_;
};

