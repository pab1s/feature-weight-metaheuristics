#include "utils/MathUtils.hpp"
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <numeric>

namespace MathUtils {
    inline std::set<int> getCategoricalIndexes(const std::vector<float>& v) {
        std::set<int> indexes;
        for (size_t i = 0; i < v.size(); ++i) {
            if (std::floor(v[i]) != v[i]) {
                indexes.insert(static_cast<int>(i));
            }
        }
        return indexes;
    }

    float normalize(float value, float min, float max) {
        if (min != max) {
            return (value - min) / (max - min);
        }
        return 0.0f;
    }

    std::vector<float> normalize(const std::vector<float>& values, const std::vector<float>& minValues, const std::vector<float>& maxValues) {
        std::vector<float> normalizedValues(values.size());
        for (size_t i = 0; i < values.size(); ++i) {
            normalizedValues[i] = normalize(values[i], minValues[i], maxValues[i]);
        }
        return normalizedValues;
    }

    void normalizeWeights(std::vector<float>& weights) {
        float maxWeight = *std::max_element(weights.begin(), weights.end());

        for (size_t i = 0; i < weights.size(); ++i) {
            if (weights[i] < 0) {
                weights[i] = 0;
            }
        }

        if (maxWeight > 0) {
            std::transform(weights.begin(), weights.end(), weights.begin(),
                           [maxWeight](float w) { return w / maxWeight; });
        }
    }

    float hammingDistance(const std::vector<float>& v1, const std::vector<float>& v2) {
        return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0f,
                                  std::plus<>(), [](float a, float b) { return std::abs(a - b); });
    }

    float weightedHammingDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights) {
        float distance = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            distance += weights[i] * std::abs(v1[i] - v2[i]);
        }
        return distance;
    }

    float minkowskiDistance(const std::vector<float>& v1, const std::vector<float>& v2, float p) {
        float sum = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            sum += std::pow(std::abs(v1[i] - v2[i]), p);
        }
        return std::pow(sum, 1.0f / p);
    }

    float weightedMinkowskiDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights, float p) {
        float sum = 0.0;
        for (size_t i = 0; i < v1.size(); ++i) {
            sum += weights[i] * std::pow(std::abs(v1[i] - v2[i]), p);
        }
        return std::pow(sum, 1.0f / p);
    }

    float calculateDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights, float p) {
        return weightedMinkowskiDistance(v1, v2, weights, p);
    }

    float weightedEuclideanDistance(const std::vector<float>& v1, const std::vector<float>& v2, const std::vector<float>& weights) {
        float sum = 0.0f;
        for (size_t i = 0; i < v1.size(); ++i) {
            float diff = v1[i] - v2[i];
            sum += weights[i] * diff * diff;
        }

        return std::sqrt(sum);
    }

    float euclideanDistance(const std::vector<float>& v1, const std::vector<float>& v2) {
        return weightedEuclideanDistance(v1, v2, std::vector<float>(v1.size(), 1.0f));
    }
}
