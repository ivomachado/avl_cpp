#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <optional>


#define LOG std::cout << __FILE__ << ":" << __LINE__ << "\n"

template <class T, class Less = std::less<T>> class Node {
public:
    enum Direction : int8_t { Left = -1, Right = +1 };
    using NodePtr = typename std::unique_ptr<Node>;
    friend std::unique_ptr<Node> std::make_unique<Node>(T&&);

    static NodePtr create(T&& value) {
        return std::make_unique<Node>(std::forward<T&&>(value));
    }

    NodePtr &getChild(Direction direction) {
        return (direction == Direction::Left ? left_ : right_);
    }

    const T &value() const { return value_; }

    bool operator<(const Node &b) const { return value() < b.value(); }

    int &fb() { return fb_; }

    bool isLeaf() const { return !(left_ || right_); };

    std::pair<NodePtr, bool> insert(NodePtr &&self, T &&value) {
        if (value == value_) {
            return std::make_pair(std::move(self), false);
        }
        auto direction = searchDirection(value);
        if (!getChild(direction)) {
            setChild(direction, std::make_unique<Node>(std::move(value)));
            fb_ += direction;
            return std::make_pair(std::move(self), fb_ != 0);
        }

        auto [newChild, childGotTaller] =
            getChild(direction)->insert(moveChild(direction), std::move(value));
        setChild(direction, std::move(newChild));

        if (!childGotTaller) {
            return std::make_pair(std::move(self), false);
        }

        fb_ += direction;
        if (std::abs(fb_) < 2) {
            return std::make_pair(std::move(self), fb_ == direction);
        }
        return std::make_pair(rotate(otherDirection(direction), std::move(self)),
                false);
    }

    std::pair<NodePtr, std::optional<T>> remove(NodePtr self, const T &value) {
        if (value == value_) {
            if (isLeaf()) {
                return std::make_pair(nullptr, std::make_optional(std::move(value_)));
            }
            if (!getChild(Direction::Left) != !getChild(Direction::Right)) {
                auto direction =
                    getChild(Direction::Left) ? Direction::Left : Direction::Right;
                return std::make_pair(moveChild(direction), std::make_optional(value_));
            }

            auto oldLeftChildFb = getChild(Direction::Left)->fb();

            auto [leftChild, replacement] =
                getChild(Direction::Left)
                ->getRightReplacement(self->moveChild(Direction::Left));

            replacement->fb() = fb();
            if (!leftChild || (oldLeftChildFb != leftChild->fb() && leftChild->fb() == 0)) {
                replacement->fb() += Direction::Right;
            }

            replacement->setChild(Direction::Left, std::move(leftChild));
            replacement->setChild(Direction::Right, moveChild(Direction::Right));

            if (std::abs(replacement->fb()) == 2) {
                return std::make_pair(rotate(Direction::Left, std::move(replacement)),
                        std::make_optional(value_));
            }

            return std::make_pair(std::move(replacement), std::make_optional(value_));
        } else {
            auto direction = searchDirection(value);

            if (auto &oldChild = getChild(direction)) {
                auto childOldFb = oldChild->fb();
                auto [newChild, data] = oldChild->remove(moveChild(direction), value);

                if (!newChild ||
                        (childOldFb != newChild->fb() && newChild->fb() == 0)) {
                    fb() += otherDirection(direction);
                }
                setChild(direction, std::move(newChild));

                if (std::abs(fb()) == 2) {
                    return std::make_pair(rotate(direction, std::move(self)),
                            std::move(data));
                }
                return std::make_pair(std::move(self), std::move(data));
            } else {
                return std::make_pair(std::move(self), std::optional<T>());
            }
        }
    }

    void print(size_t level) {
        std::cout << "fb: " << fb_ << ", value: " << value() << std::endl;

        if (left_) {
            for (size_t i = 0; i < level + 1; i++) {
                std::cout << "    ";
            }
            std::cout << "left: ";
            left_->print(level + 1);
        }

        if (right_) {
            for (size_t i = 0; i < level + 1; i++) {
                std::cout << "    ";
            }
            std::cout << "right: ";
            right_->print(level + 1);
        }
    }

private:
    Node(T &&v) : value_(v), fb_(0), left_(nullptr), right_(nullptr) {}

    T value_;
    int fb_;
    NodePtr left_, right_;

    std::pair<NodePtr, NodePtr> getRightReplacement(NodePtr self) {
        auto direction = Direction::Right;
        if (auto &oldChild = getChild(direction)) {
            auto childOldFb = oldChild->fb();
            auto [newChild, replacement] =
                oldChild->getRightReplacement(self->moveChild(direction));

            if (!newChild || (childOldFb != newChild->fb() && newChild->fb() == 0)) {
                fb() += otherDirection(direction);
            }
            setChild(direction, std::move(newChild));

            if (std::abs(fb()) == 2) {
                return std::make_pair(rotate(direction, std::move(self)),
                        std::move(replacement));
            }
            return std::make_pair(std::move(self), std::move(replacement));
        }
        auto leftChild = self->moveChild(Direction::Left);
        return std::make_pair(std::move(leftChild), std::move(self));
    }

    Direction searchDirection(const T &value) const {
        return Less()(value, value_) ? Direction::Left : Direction::Right;
    }

    static Direction otherDirection(Direction direction) {
        return static_cast<Direction>(direction * -1);
    }

    static NodePtr rotateSingle(Direction direction, NodePtr father) {
        auto child = father->moveChild(otherDirection(direction));

        if (child->fb() == 0) {
            father->fb() = otherDirection(direction);
            child->fb() = direction;
        } else {
            father->fb() = 0;
            child->fb() = 0;
        }

        father->setChild(otherDirection(direction), child->moveChild(direction));
        child->setChild(direction, std::move(father));

        return child;
    }

    static NodePtr rotateDouble(Direction direction, NodePtr father) {
        auto child = father->moveChild(otherDirection(direction));
        auto grandChild = child->moveChild(direction);

        if (grandChild->fb() == 0) {
            father->fb() = 0;
            child->fb() = 0;
        } else {
            if (grandChild->fb() == otherDirection(direction)) {
                father->fb() = direction;
                child->fb() = 0;
            } else {
                father->fb() = 0;
                child->fb() = otherDirection(direction);
            }
        }
        grandChild->fb() = 0;

        child->setChild(direction,
                grandChild->moveChild(otherDirection(direction)));
        father->setChild(otherDirection(direction),
                grandChild->moveChild(direction));

        grandChild->setChild(direction, std::move(father));
        grandChild->setChild(otherDirection(direction), std::move(child));

        return grandChild;
    }

    static NodePtr rotate(Direction direction, NodePtr father) {
        auto oppositeDirection = otherDirection(direction);
        if (father->getChild(oppositeDirection)->fb() == direction) {
            return rotateDouble(direction, std::move(father));
        } else {
            return rotateSingle(direction, std::move(father));
        }
    }

    NodePtr moveChild(Direction direction) {
        return std::move(direction == Direction::Left ? left_ : right_);
    }

    void setChild(Direction direction, NodePtr &&p) {
        (direction == Direction::Left ? left_ : right_) = std::move(p);
    }
};
