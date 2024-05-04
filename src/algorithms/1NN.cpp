#include "algorithms/1NN.hpp"
#include "utils/MathUtils.hpp"
#include <limits>
#include <omp.h>

OneNN::OneNN(const DataSet& trainingData) : KNNClassifier(trainingData, 1) {}

std::string OneNN::classify(const DataItem& item, const Solution& solution) const {
    float globalMinDistance = std::numeric_limits<float>::max();
    std::string nearestLabel;

    #pragma omp parallel
    {
        float localMinDistance = std::numeric_limits<float>::max();
        std::string localNearestLabel;

        #pragma omp for nowait
        for (size_t i = 0; i < trainingData.size(); ++i) {
            float dist = MathUtils::weightedEuclideanDistance(trainingData[i].features, item.features, solution.weights);
            if (dist < localMinDistance) {
                localMinDistance = dist;
                localNearestLabel = trainingData[i].label;
            }
        }

        #pragma omp critical
        {
            if (localMinDistance < globalMinDistance) {
                globalMinDistance = localMinDistance;
                nearestLabel = localNearestLabel;
            }
        }
    }

    return nearestLabel;
}

std::string OneNN::classifyExcludingIndex(const DataItem& item, size_t excludeIndex, const Solution& solution) {
    float minDistance = std::numeric_limits<float>::max();
    std::string nearestLabel;

    #pragma omp parallel
    {
        float localMinDistance = std::numeric_limits<float>::max();
        std::string localNearestLabel;
        #pragma omp for nowait
        for (size_t i = 0; i < trainingData.size(); ++i) {
            if (i == excludeIndex) {
                continue;
            }
            float dist = MathUtils::weightedEuclideanDistance(trainingData[i].features, item.features, solution.weights);
            if (dist < localMinDistance) {
                localMinDistance = dist;
                localNearestLabel = trainingData[i].label;
            }
        }
        #pragma omp critical
        {
            if (localMinDistance < minDistance) {
                minDistance = localMinDistance;
                nearestLabel = localNearestLabel;
            }
        }
    }

    return nearestLabel;
}