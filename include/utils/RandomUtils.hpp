#pragma once
#include <random>

namespace RandomUtils {
    /**
     * Seeds the random number generator with the given seed value.
     * 
     * @param seedValue The seed value to initialize the random number generator.
     */
    void seedRandom(unsigned int seedValue);

    /**
     * Random generator getter.
     * 
     * @return The random generator.
     */
    std::mt19937& getGenerator();

    /**
     * Generates a random integer between the specified minimum and maximum values (inclusive).
     * 
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @return A random integer between min and max (inclusive).
     */
    int getRandomInt(int min, int max);

    /**
     * Generates a random float between the specified minimum and maximum values.
     * 
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     * @return A random float between min and max.
     */
    float getRandomFloat(float min, float max);

    /**
     * Generates a random float from a normal distribution with the specified mean and standard deviation.
     * 
     * @param mean The mean value of the normal distribution.
     * @param sigma The standard deviation of the normal distribution.
     * @return A random float from a normal distribution with the specified mean and standard deviation.
     */
    float getRandomNormal(float mean, float sigma);

    /**
     * Generates a vector of shuffled indices from 0 to n-1.
     * 
     * @param n The number of indices to generate.
     * @return A vector of shuffled indices from 0 to n-1.
     */
    std::vector<size_t> generateShuffledIndices(size_t n);

    /**
     * Shuffles the elements of an integer array starting from the specified index.
     * 
     * @param array The array to shuffle.
     * @param initialIndex The index from which to start shuffling.
     * @return A shuffled copy of the input array.
     */
    std::vector<size_t> shuffleIntArray(const std::vector<size_t>& indices, size_t initialIndex = 0);
}
