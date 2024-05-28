#include "algorithms/BMB.hpp"
#include "utils/RandomUtils.hpp"

#include <algorithm>
#include <cmath>


BMB::BMB(std::shared_ptr<Evaluation> eval, std::shared_ptr<LocalSearch> ls, size_t maxIterations) 
        : eval(eval), ls(ls), maxIterations(maxIterations) {}

EvaluatedSolution BMB::run(const DataSet& dataset) {
    size_t n = dataset.getNumFeatures();
    Solution bestSolution(n);
    float bestFitness = eval->calculateFitnessLeaveOneOut(dataset, bestSolution);

    for (size_t i = 0; i < maxIterations; ++i) {
        Solution currentSolution(n);
        EvaluatedSolution currentEvaluated = ls->run(currentSolution, dataset);
        eval->insertFitnessRecord(bestFitness);

        if (currentEvaluated.fitness > bestFitness) {
            bestSolution = currentEvaluated.solution;
            bestFitness = currentEvaluated.fitness;
        }
    }

    return EvaluatedSolution{bestSolution, bestFitness};
}
