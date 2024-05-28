#pragma once

#include "algorithms/GA.hpp"

/**
 * @class SGAAC
 * @brief Represents a genetic algorithm with arithmetic crossover.
 * 
 * This class inherits from the GeneticAlgorithm class and implements the necessary methods for
 * selection, recombination, and population replacement using arithmetic crossover.
 */
class SGAAC : public GeneticAlgorithm {
public:
    /**
     * @brief Constructor for SGAArithmeticCrossover.
     * @param eval A shared pointer to the Evaluation object used for fitness evaluation.
     * @param populationSize The size of the population.
     * @param maxGenerations The maximum number of generations.
     * @param mutationRate The mutation rate.
     * @param crossoverRate The crossover rate.
     */
    SGAAC(std::shared_ptr<Evaluation> eval,
                size_t populationSize = 50,
                size_t maxGenerations = 15000,
                float mutationRate = 0.08,
                float crossoverRate = 1.0,
                float mean = 0.0f,
                float std = 0.3f)
        : GeneticAlgorithm(eval, populationSize, maxGenerations, mutationRate, crossoverRate) {}

    ~SGAAC() override {}

    /**
     * @brief Selects individuals for reproduction using tournament selection.
     * 
     * @param population The current population.
     * @return A vector of selected individuals.
     */
    std::vector<Solution> select_for_reproduction(const std::vector<Solution>& population) override;

    /**
     * @brief Mutates the population by applying random changes to the solutions.
     * 
     * @param population The current population.
     * @return A vector of mutated individuals.
     */
    std::vector<Solution> mutate_population(const std::vector<Solution>& population) override;

    /**
     * @brief Recombines the population using arithmetic crossover.
     * @param population The current population.
     * @return A vector of recombined individuals.
     */
    std::vector<Solution> recombine_population(const std::vector<Solution>& population) override;

    /**
     * @brief Replaces the current population with a new population.
     * @param population The current population.
     * @param newPopulation The new population.
     * @param dataset The dataset used for evaluation.
     */
    void replace_population(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) override;
};