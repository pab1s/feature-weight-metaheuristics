#pragma once

#include "algorithms/MA.hpp"

/**
 * @class MemeticRestart
 * @brief A class that represents a memetic algorithm with restart.
 * 
 * This class extends the MemeticAlgorithm class and adds the functionality of restarting the algorithm
 * at a certain frequency. It implements the run() method to execute the algorithm and the calculateVariance()
 * method to calculate the variance of a population.
 */
class MemeticRestart : public MemeticAlgorithm {
private:
    float restartFrequency = 0.00005f; /**< The frequency at which the algorithm should be restarted. */
public:
    /**
     * @brief Constructs a MemeticRestart object.
     * @param eval The evaluation function to be used.
     * @param ga The genetic algorithm to be used.
     * @param ls The local search algorithm to be used.
     * @param maxEvaluations The maximum number of evaluations allowed.
     * @param optimizationFrequency The frequency at which the optimization should be performed.
     * @param elitismRate The elitism rate for the genetic algorithm.
     * @param selectionRate The selection rate for the genetic algorithm.
     */
    MemeticRestart(std::shared_ptr<Evaluation> eval, std::shared_ptr<GeneticAlgorithm> ga, std::shared_ptr<LocalSearch> ls,
                    size_t maxEvaluations, size_t optimizationFrequency, float elitismRate, float selectionRate)
        : MemeticAlgorithm(eval, ga, ls, maxEvaluations, optimizationFrequency, elitismRate, selectionRate) {}

    /**
     * @brief Runs the memetic algorithm with restart.
     * @param dataset The dataset to be used for the algorithm.
     * @return The best evaluated solution found by the algorithm.
     */
    EvaluatedSolution run(const DataSet& dataset) override;

    /**
     * @brief Calculates the variance of a population.
     * @param population The population to calculate the variance for.
     * @param i The index of the solution in the population.
     * @return The variance of the population.
     */
    float calculateVariance(const std::vector<Solution>& population, size_t i);
};
