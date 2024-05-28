#pragma once

#include <memory>
#include <vector>
#include "algorithms/Algorithm.hpp"
#include "Evaluation.hpp"
#include "Solution.hpp"
#include "algorithms/GA.hpp"
#include "algorithms/LocalSearch.hpp"

/**
 * @class MemeticAlgorithm
 * @brief Represents a memetic algorithm for optimization problems.
 * 
 * The MemeticAlgorithm class is a subclass of Algorithm and implements a memetic algorithm
 * that combines genetic algorithms and local search techniques to optimize a population of solutions.
 * 
 * This class provides methods to optimize a population and run the algorithm on a given dataset.
 */
class MemeticAlgorithm : public Algorithm {
protected:
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<GeneticAlgorithm> ga;
    std::shared_ptr<LocalSearch> ls;
    std::vector<float> populationFitness;
    size_t maxEvaluations;
    size_t optimizationFrequency;
    float elitismRate;
    float selectionRate; 

public:
    /**
     * @brief Constructor for MemeticAlgorithm.
     *
     * @param eval A shared pointer to the Evaluation object used for fitness evaluation.
     * @param ga A shared pointer to the GeneticAlgorithm object used for genetic optimization.
     * @param ls A shared pointer to the LocalSearch object used for local optimization.
     * @param maxEvaluations The maximum number of evaluations.
     * @param optimizationFrequency The frequency at which the population is optimized.
     * @param elitismRate The rate of elitist selection.
     * @param selectionRate The rate of selection for reproduction.
     */
    MemeticAlgorithm(std::shared_ptr<Evaluation> eval, std::shared_ptr<GeneticAlgorithm> ga, std::shared_ptr<LocalSearch> ls,
                     size_t maxEvaluations, size_t optimizationFrequency, float elitismRate, float selectionRate);

    virtual ~MemeticAlgorithm();

    /**
     * @brief Runs the memetic algorithm on a given dataset.
     * 
     * @param dataset The dataset to optimize.
     * @return std::vector<Solution> The optimized population of solutions.
     */
    void optimize_population(std::vector<Solution>& population, const DataSet& dataset);

    /**
     * @brief Runs the memetic algorithm on a given dataset.
     * 
     * @param dataset The dataset to optimize.
     * @return EvaluatedSolution The best solution found by the algorithm.
     */
    virtual EvaluatedSolution run(const DataSet& dataset) override;
};
