#pragma once 

#include <gtest/gtest.h>
#include <random>
#include <unordered_set>

namespace avl {
namespace tests {

class StressTest {
public:
    std::default_random_engine generator;
    std::set<int> numbers;

    void populateNumbers(size_t size) {
        for(int i = 0; i < size; i++) {
            numbers.insert(i + 1);
        }
    };

    size_t getNumber() {
        std::uniform_int_distribution<size_t> distribution(0, numbers.size() - 1);
        size_t position = distribution(generator);
        auto numberIt = numbers.begin();
        std::advance(numberIt, position);
        auto number = *numberIt;
        numbers.erase(numberIt);
        return number;
    };
};

}
}
