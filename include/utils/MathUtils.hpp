#pragma once
#include <vector>
#include <set>

namespace MathUtils {
    /**
     * @brief Get the indexes of the categorical features in the given vector.
     * 
     * @param v The vector of features.
     * @return The set of indexes of the categorical features.
     */
    std::set<int> getCategoricalIndexes(const std::vector<float>& v);

    /**
     * @brief Normalize a value using the given minimum and maximum values.
     * 
     * @param value The value to be normalized.
     * @param min The minimum value.
     * @param max The maximum value.
     * @return The normalized value.
     */
    float normalize(float value, float min, float max);

    /**
     * @brief Normalize the values in the given vector using the given minimum and maximum values.
     * 
     * @param values The vector of values to be normalized.
     * @param minValues The vector of minimum values.
     * @param maxValues The vector of maximum values.
     * @return The normalized vector of values.
     */
    std::vector<float> normalize(const std::vector<float>& values, const std::vector<float>& minValues, const std::vector<float>& maxValues);

    /**
     * @brief Normalize the weights in the given vector.
     * 
     * @param weights The vector of weights to be normalized.
     */
    void normalizeWeights(std::vector<float>& weights);
    
    /**
     * @brief Calculate the Hamming distance between two vectors.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The calculated distance.
     */
    float hammingDistance(const std::vector<float>& v1, const std::vector<float>& v2);
    /**
     * @brief Calculate the weighted Hamming distance between two vectors.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param weights The vector of weights to be used in the distance calculation.
     * @return The calculated distance.
     */
    float weightedHammingDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights);
    /**
     * @brief Calculate the Minkowski distance between two vectors.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param p The value of p to be used in the distance calculation.
     * @return The calculated distance.
     */
    float minkowskiDistance(const std::vector<float>& v1, const std::vector<float>& v2, float p);
    /**
     * @brief Calculate the weighted Minkowski distance between two vectors.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param weights The vector of weights to be used in the distance calculation.
     * @param p The value of p to be used in the distance calculation.
     * @return The calculated distance.
     */
    float weightedMinkowskiDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights, float p);
    /**
     * @brief Calculate the mixed distance between two vectors.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param p The value of p to be used in the distance calculation.
     * @return The calculated distance.
     */
    float mixedDistance(const std::vector<float>& v1, const std::vector<float>& v2, float p);
    /**
     * @brief Calculate the weighted mixed distance between two vectors.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param weights The vector of weights to be used in the distance calculation.
     * @param p The value of p to be used in the distance calculation.
     * @return The calculated distance.
     */ 
    float weightedMixedDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights={}, float p = 2.0f);
    /**
     * @brief Calculate the distance between two vectors using weighted Euclidean distance.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param weights The vector of weights to be used in the distance calculation.
     * @param p The value of p to be used in the distance calculation.
     * @return The calculated distance.
     */
    float calculateDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights = {}, float p = 2.0f);

    /**
     * @brief Calculate the distance between two vectors using weighted Euclidean distance.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param weights The vector of weights to be used in the distance calculation.
     * @return The calculated distance.
     */
    float weightedEuclideanDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights);

    /**
     * @brief Calculate the Euclidean distance between two vectors.
     * 
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The calculated distance.
     */
    float euclideanDistance(const std::vector<float>& v1, const std::vector<float>& v2);
}
