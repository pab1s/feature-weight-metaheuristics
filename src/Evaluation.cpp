#include "Evaluation.hpp"
#include "utils/MathUtils.hpp"
#include <algorithm>
#include <limits>
#include <omp.h>
#include <atomic>
#include <iostream>

Evaluation::Evaluation(const DataSet& trainingData, float alpha, float reductionThreshold) : 
                        nn(trainingData), alpha(alpha), reductionThreshold(reductionThreshold) {}

std::vector<float> Evaluation::getFitnessRecords() const {
    return fitnessRecords;
}

void Evaluation::insertFitnessRecord(float fitness) {
    #pragma omp critical
    fitnessRecords.push_back(fitness);
}

Solution Evaluation::reduceSolution(const Solution& solution) {
    Solution reducedSolution = solution;
    for (size_t i = 0; i < reducedSolution.weights.size(); ++i) {
        if (reducedSolution.weights[i] < reductionThreshold) {
            reducedSolution.weights[i] = 0.0f;
        }
    }
    return reducedSolution;
}

float Evaluation::leaveOneOutCrossValidation(const DataSet& dataset, const Solution& solution) {
    std::atomic<size_t> correctPredictions{0};
    Solution reducedSolution = reduceSolution(solution);

    #pragma omp parallel for
    for (size_t i = 0; i < dataset.items.size(); ++i) {
        std::string predictedLabel = nn.classifyExcludingIndex(dataset.items[i], i, reducedSolution);
        if (predictedLabel == dataset.items[i].label) {
            correctPredictions.fetch_add(1, std::memory_order_relaxed);
        }
    }

    float classificationRate = static_cast<float>(correctPredictions.load(std::memory_order_relaxed)) / dataset.items.size() * 100.0f;
    return classificationRate;
}

float Evaluation::calculateClassificationRate(const DataSet& dataset, const Solution& solution) {
    size_t correctPredictions = 0;
    Solution reducedSolution = reduceSolution(solution);
    
    for (const auto& item : dataset.items) {        
        std::string predictedLabel = nn.classify(item, reducedSolution);
        if (predictedLabel == item.label) {
            ++correctPredictions;
        }
    }
    return static_cast<float>(correctPredictions) / dataset.items.size() * 100.0f;
}

float Evaluation::calculateReductionRate(const Solution& solution) {
    size_t reducedFeatures = 0;
    for (const auto& weight : solution.weights) {
        if (weight < 0.1f) ++reducedFeatures;
    }
    return static_cast<float>(reducedFeatures) / solution.weights.size() * 100.0f;
}

float Evaluation::calculateFitness(float classificationRate, float reductionRate) {
    return alpha * classificationRate + (1 - alpha) * reductionRate;
}

float Evaluation::calculateFitness(const DataSet& dataset, const Solution& solution) {
    float classRate = calculateClassificationRate(dataset, solution);
    float redRate = calculateReductionRate(solution);
    return calculateFitness(classRate, redRate);
}

float Evaluation::calculateFitnessLeaveOneOut(const DataSet& dataset, const Solution& solution) {
    float classRate = leaveOneOutCrossValidation(dataset, solution);
    float redRate = calculateReductionRate(solution);
    float fitness = calculateFitness(classRate, redRate);
    
    return fitness;
}

EvaluatedSolution Evaluation::evaluate(const Solution& solution, const DataSet& dataset) {
    float classRate = calculateClassificationRate(dataset, solution);
    float redRate = calculateReductionRate(solution);
    float fitness = calculateFitness(classRate, redRate);
    
    return {solution, fitness};
}

void Evaluation::clearFitnessRecords() {
    fitnessRecords.clear();
}