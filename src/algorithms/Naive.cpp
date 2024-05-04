#include "algorithms/Naive.hpp"

Naive::Naive(std::shared_ptr<Evaluation> eval) : eval(eval) {}

EvaluatedSolution Naive::run(const DataSet& dataset) {
    return eval->evaluate(Solution(dataset.getNumFeatures(), 1.0f), dataset);
}