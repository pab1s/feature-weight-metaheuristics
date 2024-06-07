#include "algorithms/ILS_SA.hpp"
#include "utils/RandomUtils.hpp"

#include <algorithm>

ILSSA::ILSSA(std::shared_ptr<Evaluation> eval, std::shared_ptr<SA> sa, size_t maxIterations, float mutationLimit)
    : eval(eval), sa(sa), maxIterations(maxIterations), mutationLimit(mutationLimit) {}

Solution ILSSA::mutate(const Solution& solution, size_t t) {
    size_t n = solution.size();
    Solution mutated = solution;

    std::vector<size_t> indices = RandomUtils::generateShuffledIndices(n);
    indices.resize(t);

    for (size_t i = 0; i < t; ++i) {
        size_t index = indices[i];
        mutated.weights.at(index) += RandomUtils::getRandomFloat(-mutationLimit, mutationLimit);
        mutated.weights.at(index) = std::clamp(mutated.weights.at(index), 0.0f, 1.0f);
    }

    return mutated;
}

EvaluatedSolution ILSSA::run(const DataSet& dataset) {
    size_t n = dataset.getNumFeatures();
    size_t t = 0.20f * n > 3 ? 3 : 0.20f * n;
    Solution currentSolution(n);
    float currentFitness = eval->calculateFitnessLeaveOneOut(dataset, currentSolution);

    for (size_t i = 0; i < maxIterations; ++i) {
        eval->insertFitnessRecord(currentFitness);
        Solution mutated = mutate(currentSolution, t);
        EvaluatedSolution evaluated = sa->run(mutated, dataset);

        if (evaluated.fitness > currentFitness) {
            currentSolution = evaluated.solution;
            currentFitness = evaluated.fitness;
        }
    }

    return EvaluatedSolution{currentSolution, currentFitness};
}