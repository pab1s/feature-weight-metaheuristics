#include "algorithms/BestLocalSearch.hpp"

#include <omp.h>
#include <limits>

BestLocalSearch::BestLocalSearch(std::shared_ptr<Evaluation> eval, size_t maxNeighbors, size_t maxEvaluations, float mean, float variance)
: LocalSearch(eval, maxNeighbors, maxEvaluations, mean, variance) {}

EvaluatedSolution BestLocalSearch::run(const DataSet& dataset) {
    size_t n = dataset.getNumFeatures();
    size_t numNeighbors = maxNeighbors * n;
    Solution currentSolution(n);
    float currentFitness = eval->calculateFitness(dataset, currentSolution);

    while (evaluations < maxEvaluations) {
        Solution bestNeighbor(n);
        float bestNeighborFitness = std::numeric_limits<float>::lowest();
        bool foundBetter = false;
        size_t index = 0;

        #pragma omp parallel for schedule(dynamic) reduction(+:evaluations)
        for (int i = 0; i < static_cast<int>(numNeighbors); ++i) {
            index = i % n;
            Solution neighbor = generateNeighbor(currentSolution, index);
            float fitness = eval->calculateFitness(dataset, neighbor);
            eval->insertFitnessRecord(currentFitness);
            evaluations++;

            #pragma omp critical
            {
                if (fitness > bestNeighborFitness) {
                    bestNeighbor = neighbor;
                    bestNeighborFitness = fitness;
                    foundBetter = true;
                }
            }
        }

        if (foundBetter) {
            currentSolution = bestNeighbor;
            currentFitness = bestNeighborFitness;
        } else {
            break;
        }
    }

    return EvaluatedSolution{currentSolution, currentFitness};
}
