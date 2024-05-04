#include "algorithms/LocalSearch.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

#include "utils/MathUtils.hpp"
#include "utils/RandomUtils.hpp"

LocalSearch::LocalSearch(std::shared_ptr<Evaluation> eval, size_t maxNeighbors, 
    size_t maxEvaluations, float mean, float variance) : eval(eval), 
    maxNeighbors(maxNeighbors), maxEvaluations(maxEvaluations), 
    std(std::sqrt(variance)) {}

Solution LocalSearch::generateNeighbor(const Solution& solution, size_t index) {
    Solution neighbor = solution;
    neighbor.weights.at(index) += RandomUtils::getRandomNormal(mean, std);
    neighbor.weights.at(index) = std::clamp(neighbor.weights[index], 0.0f, 1.0f);
    return neighbor;
}

EvaluatedSolution LocalSearch::run(Solution solution, const DataSet& dataset) {
    size_t n = dataset.getNumFeatures();
    Solution currentSolution = solution;
    float currentFitness = eval->calculateFitnessLeaveOneOut(dataset, currentSolution);

    size_t maxNumNeighbors = maxNeighbors * n; 
    size_t numNeighbors = 0;

    while (evaluations < maxEvaluations && numNeighbors < maxNumNeighbors) {
        bool improved = false;
        std::vector<size_t> indices = RandomUtils::generateShuffledIndices(n);

        for (size_t i = 0; i < indices.size() && !improved; ++i) {
            Solution neighbor = generateNeighbor(currentSolution, indices[i]);
            float fitness = eval->calculateFitnessLeaveOneOut(dataset, neighbor);
            eval->insertFitnessRecord(currentFitness);
            evaluations++;

            if (fitness > currentFitness) {
                currentSolution = neighbor;
                currentFitness = fitness;
                improved = true;
                numNeighbors = 0;
            }
        }

        if (!improved) {
            numNeighbors++;
        }
    }

    return EvaluatedSolution{currentSolution, currentFitness};
}

EvaluatedSolution LocalSearch::run(const DataSet& dataset) {
    size_t n = dataset.getNumFeatures();
    Solution currentSolution(n);
    return run(currentSolution, dataset);
}
