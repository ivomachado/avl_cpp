#include <avl/Tree.h>
#include <gtest/gtest.h>

#include <random>

#include "Util.h"

namespace avl {
namespace tests {

TEST(SetTest, InsertAndRemoval) {
    Set<int> set;
    set.insert(3);
    auto removed = set.remove(3);
    ASSERT_TRUE(removed);
    EXPECT_EQ(*removed, 3);
}

TEST(MapTest, InsertAndRemoval) {
    Tree<int, std::string> set;
    set.insert(3, std::string("oi"));
    auto removed = set.remove(3);
    ASSERT_TRUE(removed);
    EXPECT_EQ(*removed, "oi");
}

}
}
