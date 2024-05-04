#pragma once

#include "algorithms/GA.hpp"

/**
 * @class GGABLX
 * @brief A class that represents a Generational Genetic Algorithm with BLX crossover operator for continuous optimization.
 * 
 * This class inherits from the GeneticAlgorithm class and implements the necessary methods
 * for selection, recombination, and population replacement using the BLX crossover operator.
 */
class GGABLX : public GeneticAlgorithm {
protected:
    float alpha;
public:
    /**
     * @brief Constructor for GGABLX.
     * 
     * @param eval A shared pointer to the Evaluation object used for fitness evaluation.
     * @param populationSize The size of the population.
     * @param maxGenerations The maximum number of generations.
     * @param mutationRate The mutation rate.
     * @param crossoverRate The crossover rate.
     * @param alpha The alpha parameter for BLX crossover.
     */
    GGABLX(std::shared_ptr<Evaluation> eval,
           size_t populationSize = 50,
           size_t maxGenerations = 15000,
           float mutationRate = 0.08,
           float crossoverRate = 0.68,
           float alpha = 0.3f,
           float mean = 0.0f,
           float std = 0.3f)
        : GeneticAlgorithm(eval, populationSize, maxGenerations, mutationRate, crossoverRate) {alpha = alpha;}

    ~GGABLX() override {}

    /**
     * @brief Mutates the population by applying random changes to the solutions.
     * 
     * @param population The current population.
     * @return A vector of mutated individuals.
     */
    std::vector<Solution> mutate_population(const std::vector<Solution>& population) override;

    /**
     * @brief Selects individuals from the population for reproduction.
     * 
     * @param population The current population.
     * @return A vector of selected individuals.
     */
    std::vector<Solution> select_for_reproduction(const std::vector<Solution>& population) override;

    /**
     * @brief Recombines the population using the BLX crossover operator.
     * 
     * @param population The current population.
     * @return A vector of recombined individuals.
     */
    std::vector<Solution> recombine_population(const std::vector<Solution>& population) override;

    /**
     * @brief Replaces the current population with the new population.
     * 
     * @param population The current population.
     * @param newPopulation The new population.
     * @param dataset The dataset used for replacement.
     */
    void replace_population(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) override;
};