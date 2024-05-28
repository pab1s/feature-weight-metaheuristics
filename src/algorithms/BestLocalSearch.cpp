#include "algorithms/BestLocalSearch.hpp"
#include "utils/RandomUtils.hpp"

#include <omp.h>
#include <limits>

BestLocalSearch::BestLocalSearch(std::shared_ptr<Evaluation> eval, size_t maxNeighbors, size_t maxEvaluations, float mean, float variance)
: LocalSearch(eval, maxNeighbors, maxEvaluations, mean, variance) {}

EvaluatedSolution BestLocalSearch::run(Solution initialSolution, const DataSet& dataset) {
  size_t n = dataset.getNumFeatures();
  size_t maxNumNeighbors = maxNeighbors * n;

  Solution currentSolution = initialSolution;
  float currentFitness = eval->calculateFitnessLeaveOneOut(dataset, currentSolution);

  while (evaluations < maxEvaluations) {
    std::vector<size_t> indices = RandomUtils::generateShuffledIndices(n);

    // Parallel section for neighbor evaluation
    #pragma omp parallel for schedule(dynamic) reduction(+:evaluations)
    for (size_t i = 0; i < indices.size(); ++i) {
      Solution neighbor = generateNeighbor(currentSolution, indices[i]);
      float fitness = eval->calculateFitnessLeaveOneOut(dataset, neighbor);
      evaluations++;

      // Critical section for updating best solution
      #pragma omp critical
      {
        if (fitness > currentFitness) {
          currentSolution = neighbor;
          currentFitness = fitness;
        }
      }
    }

    // Check if improvement was found
    if (evaluations >= maxNumNeighbors) {
      break;
    }
  }

  return EvaluatedSolution{currentSolution, currentFitness};
}

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
