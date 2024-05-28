#pragma once

#include "algorithms/Algorithm.hpp"

#include <memory>

class SA : public Algorithm {
private:
    std::shared_ptr<Evaluation> eval;
    size_t maxEvaluations;
    float finalTemperature;
    float acceptanceProbability;
    float worseningProbability;

    /**
     * @brief Generates a neighbor of the given solution.
     * 
     * @param solution Solution to generate a neighbor from
     * @return The generated neighbor.
     */
    Solution generateNeighbor(const Solution& solution);

public:
    /**
     * @brief Constructor of the SA class.
     * 
     * @param eval Evaluation object used to evaluate solutions
     * @param maxEvaluations Maximum number of evaluations to run the algorithm
     * @param finalTemperature Final temperature of the algorithm
     * @param acceptanceProbability Probability of accepting a worse solution
     * @param worseningProbability Probability of worsening the solution
     */
    SA(std::shared_ptr<Evaluation> eval, size_t maxEvaluations, float finalTemperature = 0.001f, float acceptanceProbability = 0.3f, float worseningProbability = 0.1f);

    /**
     * @brief Runs the SA algorithm on the given dataset.
     * 
     * @param dataset Dataset to run the algorithm on
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution run(const DataSet& dataset) override;

    /**
     * @brief Runs the SA algorithm on the given initial solution and dataset.
     * 
     * @param initialSolution Initial solution to start the algorithm
     * @param dataset Dataset to run the algorithm on
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution run(const Solution& initialSolution, const DataSet& dataset);
};