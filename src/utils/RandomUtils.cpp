#include "utils/random.hpp"
#include <vector>
#include <algorithm>

namespace RandomUtils {
    using Random = effolkronium::random_static;

    void seedRandom(unsigned int seedValue) {
        Random::seed(seedValue);
    }

    int getRandomInt(int min, int max) {
        return Random::get(min, max);
    }

    float getRandomFloat(float min, float max) {
        return Random::get(min, max);
    }

    float getRandomNormal(float mean, float sigma) {
        return Random::get<std::normal_distribution<>>(mean, sigma);
    }

    std::vector<size_t> generateShuffledIndices(size_t n) {
        std::vector<size_t> indices(n);
        std::iota(indices.begin(), indices.end(), 0);
        Random::shuffle(indices);
        return indices;
    }

    std::vector<size_t> shuffleIntArray(const std::vector<size_t>& array, size_t initialIndex) {
        std::vector<size_t> shuffledArray(array);
        std::shuffle(shuffledArray.begin() + initialIndex, shuffledArray.end(), Random::engine());
        return shuffledArray;
    }
    
}
