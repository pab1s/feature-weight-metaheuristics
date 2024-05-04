#pragma once

#include <memory>
#include <vector>
#include "algorithms/Algorithm.hpp"
#include "Evaluation.hpp"
#include "Solution.hpp"
#include "algorithms/GA.hpp"
#include "algorithms/LocalSearch.hpp"

class MemeticAlgorithm : public Algorithm {
protected:
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<GeneticAlgorithm> ga;
    std::shared_ptr<LocalSearch> ls;
    std::vector<float> populationFitness;
    size_t maxGenerations;
    size_t optimizationFrequency;
    float elitismRate;
    float selectionRate; 

public:
    MemeticAlgorithm(std::shared_ptr<Evaluation> eval, std::shared_ptr<GeneticAlgorithm> ga, std::shared_ptr<LocalSearch> ls,
                     size_t maxGenerations, size_t optimizationFrequency, float elitismRate, float selectionRate);

    virtual ~MemeticAlgorithm();

    void optimize_population(std::vector<Solution>& population, const DataSet& dataset);

    EvaluatedSolution run(const DataSet& dataset) override;
};
