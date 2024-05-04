#include "algorithms/KNNClassifier.hpp"
#include "utils/MathUtils.hpp"
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <functional>
#include <iostream>
#include <omp.h>

KNNClassifier::KNNClassifier(const DataSet& originalTrainingData, size_t k) : trainingData(originalTrainingData), k(k) {}

size_t KNNClassifier::getK() const {
    return k;
}

std::string KNNClassifier::classify(const DataItem& testItem, const Solution& solution) const {
    std::vector<std::pair<float, std::string>> distances(trainingData.items.size());
    
    #pragma omp parallel for
    for (size_t i = 0; i < static_cast<size_t>(trainingData.items.size()); ++i) {
        const auto& item = trainingData.items[i];
        float dist = MathUtils::weightedEuclideanDistance(item.features, testItem.features, solution.weights);
        distances[i] = {dist, item.label};
    }
    
    std::sort(distances.begin(), distances.end());
    std::unordered_map<std::string, size_t> labelCounts;
    for (size_t i = 0; i < k && i < distances.size(); ++i) {
        labelCounts[distances[i].second]++;
    }

    std::string mostCommonLabel;
    size_t highestCount = 0;
    for (const auto& pair : labelCounts) {
        if (pair.second > highestCount) {
            highestCount = pair.second;
            mostCommonLabel = pair.first;
        }
    }

    return mostCommonLabel;
}

std::string KNNClassifier::classifyExcludingIndex(const DataItem& item, size_t excludeIndex, const Solution& solution) {
    std::vector<std::pair<float, std::string>> distances;
    
    #pragma omp parallel
    {
        std::vector<std::pair<float, std::string>> local_distances;
        #pragma omp for nowait
        for (size_t i = 0; i < trainingData.items.size(); ++i) {
            if (i == excludeIndex) {
                continue;
            }
            float dist = MathUtils::calculateDistance(trainingData.items[i].features, item.features, solution.weights);
            local_distances.emplace_back(dist, trainingData.items[i].label);
        }
        #pragma omp critical
        distances.insert(distances.end(), local_distances.begin(), local_distances.end());
    }
    
    std::sort(distances.begin(), distances.end());

    std::unordered_map<std::string, size_t> labelCounts;
    for (size_t i = 0; i < k && i < distances.size(); ++i) {
        labelCounts[distances[i].second]++;
    }

    std::string mostCommonLabel;
    size_t highestCount = 0;
    for (const auto& pair : labelCounts) {
        if (pair.second > highestCount) {
            highestCount = pair.second;
            mostCommonLabel = pair.first;
        }
    }

    return mostCommonLabel;
}
