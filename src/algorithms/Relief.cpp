#include "algorithms/Relief.hpp"
#include "utils/MathUtils.hpp"
#include "data/DataItem.hpp"
#include "data/DataSet.hpp"
#include <limits>
#include <vector>
#include <iostream>

Relief::Relief(std::shared_ptr<Evaluation> eval) : eval(eval) {}

EvaluatedSolution Relief::run(const DataSet& dataset) {
    Solution solution(dataset.getNumFeatures(), 0.0f);

    for (const auto& instance : dataset.items) {
        bool foundFriend = false, foundEnemy = false;
        DataItem closestFriend, closestEnemy;
        float closestFriendDist = std::numeric_limits<float>::max();
        float closestEnemyDist = std::numeric_limits<float>::max();

        for (const auto& other : dataset.items) {
            if (&instance != &other) {
            
            float dist = MathUtils::euclideanDistance(instance.features, other.features);
            if (instance.label == other.label && dist < closestFriendDist) {
                closestFriend = other;
                closestFriendDist = dist;
                foundFriend = true;
            } else if (instance.label != other.label && dist < closestEnemyDist) {
                closestEnemy = other;
                closestEnemyDist = dist;
                foundEnemy = true;
            }
            }
        }

        if (foundFriend && foundEnemy) {
            for (size_t i = 0; i < solution.weights.size(); ++i) {
                float diffEnemy = std::abs(instance.features[i] - closestEnemy.features[i]);
                float diffFriend = std::abs(instance.features[i] - closestFriend.features[i]);
                solution.weights[i] += diffEnemy - diffFriend;
            }
        }
    }

    MathUtils::normalizeWeights(solution.weights);

    return eval->evaluate(solution, dataset);
}
