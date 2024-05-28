#include <iostream>
#include "Solution.hpp"
#include "utils/RandomUtils.hpp"

Solution::Solution(size_t numberOfFeatures) : weights(numberOfFeatures) {
    initializeRandomly();
}
Solution::Solution(size_t numberOfFeatures, float value) : weights(numberOfFeatures, value) {}

void Solution::initializeRandomly() {
    for (auto& weight : weights) {
        weight = RandomUtils::getRandomFloat(0.0f, 1.0f);
    }
}

float Solution::getWeight(int index) const {
    return weights.at(index);
}

void Solution::setWeight(int index, float value) {
    weights[index] = value;
}

size_t Solution::size() const {
    return weights.size();
}

Solution& Solution::operator=(const Solution& other) {
    if (this != &other) {
        weights = other.weights;
    }
    return *this;
}

Solution Solution::operator+(const Solution& other) const {
    Solution result(weights.size());
    for (size_t i = 0; i < weights.size(); ++i) {
        result.setWeight(i, weights[i] + other.weights[i]);
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Solution& solution) {
    os << "[";
    for (size_t i = 0; i < solution.size(); ++i) {
        os << solution.getWeight(i);
        if (i < solution.size() - 1) {
            os << ",";
        }
    }
    os << "]";

    return os;
}
