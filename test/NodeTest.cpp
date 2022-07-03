#include <avl/Node.h>
#include <gtest/gtest.h>
#include <random>
#include <unordered_set>

#include "Util.h"

namespace avl {
namespace tests {

using NodeInt = Node<int>;
using NodeIntWithString = Node<int, std::string>;

TEST(NodeTest, Construction) {
    for (int i = 0; i < 1000; i++) {
        auto node = NodeInt::create(1);
        EXPECT_TRUE(node->isLeaf());
        EXPECT_EQ(node->getChild(NodeInt::Direction::Left), nullptr);
        EXPECT_EQ(node->getChild(NodeInt::Direction::Right), nullptr);
        EXPECT_EQ(node->fb(), 0);
        EXPECT_EQ(node->key(), 1);
    }
}

TEST(NodeTest, ConstructionWithValue) {
    for (int i = 0; i < 1000; i++) {
        auto node = NodeIntWithString::create(1, std::string("string") + std::to_string(i));
        EXPECT_EQ(node->value(), std::string("string") + std::to_string(i));
        node->value() = std::string("string") + std::to_string(i + 1);
        EXPECT_EQ(node->value(), std::string("string") + std::to_string(i + 1));
    }
}

TEST(NodeTest, InsertionWithValue) {
    for (int i = 0; i < 1000; i++) {
        auto node = NodeIntWithString::create(1, std::string("string") + std::to_string(i));
        std::tie(node, std::ignore) = node->insert(std::move(node), i, std::string("string") + std::to_string(i));
        EXPECT_EQ(node->value(), std::string("string") + std::to_string(i));
        node->value() = std::string("string") + std::to_string(i + 1);
        EXPECT_EQ(node->value(), std::string("string") + std::to_string(i + 1));
    }
}

TEST(NodeTest, RemoveWithValue) {
    for (int i = 0; i < 1000; i++) {
        auto node = NodeIntWithString::create(1, std::string("string") + std::to_string(i));
        auto [newNode, value] = node->remove(std::move(node), 1);
        ASSERT_TRUE(value);
        EXPECT_EQ(*value, std::string("string") + std::to_string(i));
        EXPECT_EQ(newNode, nullptr);
    }
}

TEST(NodeTest, Sizes) {
    EXPECT_LT(sizeof(Node<int8_t>), sizeof(Node<int8_t, int64_t>));
    EXPECT_EQ(sizeof(Node<int16_t>), sizeof(Node<int8_t, int8_t>));
    EXPECT_LT(sizeof(Node<int8_t>), sizeof(Node<int64_t>));
    EXPECT_LT(sizeof(Node<int64_t>), sizeof(Node<std::tuple<int64_t, int64_t>>));
}

TEST(NodeTest, InsertBiggerAndSmaller) {
    auto node = NodeInt::create(2);
    auto oldAddress = node.get();

    bool gotTaller;
    std::tie(node, gotTaller) = node->insert(std::move(node), 3);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), 1);
    EXPECT_TRUE(gotTaller);

    std::tie(node, gotTaller) = node->insert(std::move(node), 1);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), 0);
    EXPECT_FALSE(gotTaller);

    decltype(auto) leftChild = node->getChild(NodeInt::Direction::Left);
    decltype(auto) rightChild = node->getChild(NodeInt::Direction::Right);

    ASSERT_NE(leftChild, nullptr);
    ASSERT_NE(rightChild, nullptr);

    EXPECT_EQ(leftChild->key(), 1);
    EXPECT_EQ(rightChild->key(), 3);
}

TEST(NodeTest, InsertSmallerAndBigger) {
    auto node = NodeInt::create(2);
    auto oldAddress = node.get();

    bool gotTaller;
    std::tie(node, gotTaller) = node->insert(std::move(node), 1);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), -1);
    EXPECT_TRUE(gotTaller);

    std::tie(node, gotTaller) = node->insert(std::move(node), 3);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), 0);
    EXPECT_FALSE(gotTaller);

    decltype(auto) leftChild = node->getChild(NodeInt::Direction::Left);
    decltype(auto) rightChild = node->getChild(NodeInt::Direction::Right);

    ASSERT_NE(leftChild, nullptr);
    ASSERT_NE(rightChild, nullptr);

    EXPECT_EQ(leftChild->key(), 1);
    EXPECT_EQ(rightChild->key(), 3);
}

TEST(NodeTest, SingleRotationRight) {
    auto node = NodeInt::create(1);
    auto oldAddress = node.get();

    bool gotTaller;
    std::tie(node, gotTaller) = node->insert(std::move(node), 2);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), 1);
    EXPECT_TRUE(gotTaller);

    std::tie(node, gotTaller) = node->insert(std::move(node), 3);

    ASSERT_NE(node.get(), oldAddress);
    EXPECT_EQ(node->key(), 2);
    EXPECT_EQ(node->fb(), 0);
    EXPECT_FALSE(gotTaller);

    decltype(auto) leftChild = node->getChild(NodeInt::Direction::Left);
    decltype(auto) rightChild = node->getChild(NodeInt::Direction::Right);

    ASSERT_NE(leftChild, nullptr);
    ASSERT_NE(rightChild, nullptr);

    EXPECT_EQ(leftChild->key(), 1);
    EXPECT_EQ(leftChild.get(), oldAddress);
    EXPECT_EQ(leftChild->fb(), 0);
    EXPECT_EQ(rightChild->fb(), 0);
    EXPECT_EQ(rightChild->key(), 3);
}

TEST(NodeTest, SingleRotationLeft) {
    auto node = NodeInt::create(3);
    auto oldAddress = node.get();

    bool gotTaller;
    std::tie(node, gotTaller) = node->insert(std::move(node), 2);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), -1);
    EXPECT_TRUE(gotTaller);

    std::tie(node, gotTaller) = node->insert(std::move(node), 1);

    ASSERT_NE(node.get(), oldAddress);
    EXPECT_EQ(node->key(), 2);
    EXPECT_EQ(node->fb(), 0);
    EXPECT_FALSE(gotTaller);

    decltype(auto) leftChild = node->getChild(NodeInt::Direction::Left);
    decltype(auto) rightChild = node->getChild(NodeInt::Direction::Right);

    ASSERT_NE(leftChild, nullptr);
    ASSERT_NE(rightChild, nullptr);

    EXPECT_EQ(leftChild->key(), 1);
    EXPECT_EQ(rightChild.get(), oldAddress);
    EXPECT_EQ(leftChild->fb(), 0);
    EXPECT_EQ(rightChild->fb(), 0);
    EXPECT_EQ(rightChild->key(), 3);
}

TEST(NodeTest, DoubleRotationRight) {
    auto node = NodeInt::create(3);
    auto oldAddress = node.get();

    bool gotTaller;
    std::tie(node, gotTaller) = node->insert(std::move(node), 1);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), -1);
    EXPECT_TRUE(gotTaller);

    std::tie(node, gotTaller) = node->insert(std::move(node), 2);

    ASSERT_NE(node.get(), oldAddress);
    EXPECT_EQ(node->key(), 2);
    EXPECT_EQ(node->fb(), 0);
    EXPECT_FALSE(gotTaller);

    decltype(auto) leftChild = node->getChild(NodeInt::Direction::Left);
    decltype(auto) rightChild = node->getChild(NodeInt::Direction::Right);

    ASSERT_NE(leftChild, nullptr);
    ASSERT_NE(rightChild, nullptr);

    EXPECT_EQ(leftChild->key(), 1);
    EXPECT_EQ(rightChild.get(), oldAddress);
    EXPECT_EQ(rightChild->fb(), 0);
    EXPECT_EQ(rightChild->key(), 3);
}

TEST(NodeTest, DoubleRotationLeft) {
    auto node = NodeInt::create(1);
    auto oldAddress = node.get();

    bool gotTaller;
    std::tie(node, gotTaller) = node->insert(std::move(node), 3);

    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), 1);
    EXPECT_TRUE(gotTaller);

    std::tie(node, gotTaller) = node->insert(std::move(node), 2);

    ASSERT_NE(node.get(), oldAddress);
    EXPECT_EQ(node->key(), 2);
    EXPECT_EQ(node->fb(), 0);
    EXPECT_FALSE(gotTaller);

    decltype(auto) leftChild = node->getChild(NodeInt::Direction::Left);
    decltype(auto) rightChild = node->getChild(NodeInt::Direction::Right);

    ASSERT_NE(leftChild, nullptr);
    ASSERT_NE(rightChild, nullptr);

    EXPECT_EQ(leftChild->key(), 1);
    EXPECT_EQ(leftChild.get(), oldAddress);
    EXPECT_EQ(leftChild->fb(), 0);
    EXPECT_EQ(rightChild->key(), 3);
}

TEST(NodeTest, RemoveLeaf) {
    auto node = NodeInt::create(1);

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 1);

    ASSERT_TRUE(receivedValue);
    EXPECT_EQ(*receivedValue, 1);

    EXPECT_EQ(node, nullptr);
}

TEST(NodeTest, RemoveRightChild) {
    auto node = NodeInt::create(1);
    auto oldAddress = node.get();

    std::tie(node, std::ignore) = node->insert(std::move(node), 2);

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 2);

    ASSERT_TRUE(receivedValue);
    EXPECT_EQ(*receivedValue, 2);

    EXPECT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), 0);
}

TEST(NodeTest, RemoveLeftChild) {
    auto node = NodeInt::create(2);
    auto oldAddress = node.get();

    std::tie(node, std::ignore) = node->insert(std::move(node), 1);

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 1);

    ASSERT_TRUE(receivedValue);
    EXPECT_EQ(*receivedValue, 1);

    EXPECT_EQ(node.get(), oldAddress);
    EXPECT_EQ(node->fb(), 0);
}

TEST(NodeTest, RemoveNodeWithLeftChild) {
    auto node = NodeInt::create(2);

    std::tie(node, std::ignore) = node->insert(std::move(node), 1);
    auto leftChildAddress = node->getChild(NodeInt::Direction::Left).get();

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 2);

    ASSERT_TRUE(receivedValue);
    EXPECT_EQ(*receivedValue, 2);

    ASSERT_EQ(node.get(), leftChildAddress);
    EXPECT_EQ(node->fb(), 0);
}

TEST(NodeTest, RemoveNodeWithRightChild) {
    auto node = NodeInt::create(2);

    std::tie(node, std::ignore) = node->insert(std::move(node), 3);
    auto rightChildAddress = node->getChild(NodeInt::Direction::Right).get();

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 2);

    ASSERT_TRUE(receivedValue);
    EXPECT_EQ(*receivedValue, 2);

    ASSERT_EQ(node.get(), rightChildAddress);
    EXPECT_EQ(node->fb(), 0);
}

TEST(NodeTest, RemoveNodeWithTwoChildren) {
    auto node = NodeInt::create(2);

    std::tie(node, std::ignore) = node->insert(std::move(node), 1);
    std::tie(node, std::ignore) = node->insert(std::move(node), 3);
    auto leftChildAddress = node->getChild(NodeInt::Direction::Left).get();

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 2);

    ASSERT_TRUE(receivedValue);
    ASSERT_EQ(node.get(), leftChildAddress);
    EXPECT_EQ(node->fb(), 1);
    EXPECT_EQ(*receivedValue, 2);
}

TEST(NodeTest, RemoveNodeWithTwoChildrenAndGrandchild) {
    auto node = NodeInt::create(3);

    std::tie(node, std::ignore) = node->insert(std::move(node), 1);
    std::tie(node, std::ignore) = node->insert(std::move(node), 4);
    std::tie(node, std::ignore) = node->insert(std::move(node), 2);

    ASSERT_EQ(node->fb(), -1);
    ASSERT_NE(node->getChild(NodeInt::Direction::Left), nullptr);

    auto expectedRoot = node->getChild(NodeInt::Direction::Left)->getChild(NodeInt::Direction::Right).get();
    ASSERT_NE(expectedRoot, nullptr);

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 3);

    ASSERT_TRUE(receivedValue);
    ASSERT_EQ(node.get(), expectedRoot);
    EXPECT_EQ(*receivedValue, 3);

    EXPECT_EQ(node->fb(), 0);
}

class NodeComplexTest: public ::testing::Test {
public:
    NodeInt::NodePtr node;

    NodeComplexTest(): node(NodeInt::create(5)) {};

    void SetUp() override {
        std::tie(node, std::ignore) = node->insert(std::move(node), 6);
        std::tie(node, std::ignore) = node->insert(std::move(node), 2);
        std::tie(node, std::ignore) = node->insert(std::move(node), 3);
        std::tie(node, std::ignore) = node->insert(std::move(node), 1);
        std::tie(node, std::ignore) = node->insert(std::move(node), 7);
        std::tie(node, std::ignore) = node->insert(std::move(node), 4);

        ASSERT_NE(node->getChild(NodeInt::Direction::Left), nullptr);
    };
};

TEST_F(NodeComplexTest, RemoveRoot) {
    ASSERT_NE(node->getChild(NodeInt::Direction::Left)->getChild(NodeInt::Direction::Right)->getChild(NodeInt::Direction::Right), nullptr);

    auto expectedRoot = node->getChild(NodeInt::Direction::Left)->getChild(NodeInt::Direction::Right)->getChild(NodeInt::Direction::Right).get();
    ASSERT_NE(expectedRoot, nullptr);

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 5);

    ASSERT_TRUE(receivedValue);
    ASSERT_EQ(node.get(), expectedRoot);
    EXPECT_EQ(*receivedValue, 5);

    EXPECT_EQ(node->fb(), 0);
}

TEST_F(NodeComplexTest, RemoveNode2) {
    ASSERT_NE(node->getChild(NodeInt::Direction::Left)->getChild(NodeInt::Direction::Left), nullptr);
    auto oldAddress = node.get();

    // a Single Left rotation is made
    auto expectedLeftChild = node->getChild(NodeInt::Direction::Left)->getChild(NodeInt::Direction::Right).get();
    ASSERT_NE(expectedLeftChild, nullptr);

    std::optional<int> receivedValue;
    std::tie(node, receivedValue) = node->remove(std::move(node), 2);

    ASSERT_TRUE(receivedValue);
    ASSERT_EQ(node.get(), oldAddress);
    EXPECT_EQ(*receivedValue, 2);

    EXPECT_EQ(node->fb(), 0);

    decltype(auto) leftChild = node->getChild(NodeInt::Direction::Left);
    EXPECT_EQ(leftChild.get(), expectedLeftChild);
}

class NodeStressTest: public ::testing::Test, public StressTest {
public:
    NodeInt::NodePtr node;

    NodeStressTest(): StressTest(),
    node(NodeInt::create(5)) {};

    size_t checkOrderAndReturnSize (const NodeInt::NodePtr& node) {
        size_t leftSubTreeHeight = 0, rightSubTreeHeight = 0;
        if (decltype(auto) child = node->getChild(NodeInt::Direction::Left)) {
            EXPECT_LT(child->key(), node->key());
            leftSubTreeHeight = checkOrderAndReturnSize(child);
        }

        if (decltype(auto) child = node->getChild(NodeInt::Direction::Right)) {
            EXPECT_GT(child->key(), node->key());
            rightSubTreeHeight = checkOrderAndReturnSize(child);
        }
        EXPECT_EQ(node->fb(), rightSubTreeHeight - leftSubTreeHeight);
        EXPECT_LE(abs(node->fb()), 1);
        return std::max(leftSubTreeHeight, rightSubTreeHeight) + 1;
    };

    void testInsertion(size_t size) {
        populateNumbers(size);
        auto node = NodeInt::create(getNumber());

        while(!numbers.empty()) {
            std::tie(node, std::ignore) = node->insert(std::move(node), getNumber());
            auto height = checkOrderAndReturnSize(node);

            EXPECT_LE(height, std::ceil(1.44 * std::ceil(std::log(size - numbers.size())) / std::log(2)));
        }
    }

    void testInsertionFast(size_t size) {
        populateNumbers(size);
        auto node = NodeInt::create(getNumber());

        while(!numbers.empty()) {
            std::tie(node, std::ignore) = node->insert(std::move(node), getNumber());
        }
        auto height = checkOrderAndReturnSize(node);

        EXPECT_LE(height, std::ceil(1.44 * std::ceil(std::log(size - numbers.size())) / std::log(2)));
    }

    void testRemotion(size_t size) {
        populateNumbers(size);
        auto node = NodeInt::create(getNumber());

        while(!numbers.empty()) {
            std::tie(node, std::ignore) = node->insert(std::move(node), getNumber());
        }

        populateNumbers(size);

        while(!numbers.empty()) {
            std::optional<int> receivedValue;
            auto number = getNumber();
            std::tie(node, receivedValue) = node->remove(std::move(node), number);
            ASSERT_TRUE(receivedValue);
            EXPECT_EQ(number, *receivedValue);

            if (node) {
                auto height = checkOrderAndReturnSize(node);

                if (height > 1) {
                    EXPECT_LE(height, std::ceil(1.44 * std::ceil(std::log(numbers.size())) / std::log(2)));
                }
            } else {
                EXPECT_TRUE(numbers.empty());
            }
        }
    }
};

#define TEST_INSERTION(SIZE) \
TEST_F(NodeStressTest, TestInsertionWith##SIZE) { \
    testInsertion(SIZE); \
}
#define TEST_INSERTION_FAST(SIZE) \
TEST_F(NodeStressTest, TestInsertionFastWith##SIZE) { \
    testInsertionFast(SIZE); \
}


#define TEST_REMOTION(SIZE) \
TEST_F(NodeStressTest, TestRemotionWith##SIZE) { \
    testRemotion(SIZE); \
}

#define TEST_INSERTION_AND_REMOTION(SIZE) \
TEST_INSERTION(SIZE); \
TEST_REMOTION(SIZE)

TEST_INSERTION_AND_REMOTION(100);
TEST_INSERTION_AND_REMOTION(1000);
TEST_INSERTION_AND_REMOTION(10000);
TEST_INSERTION_FAST(100000);

}
}
