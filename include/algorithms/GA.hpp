#pragma once

#include <memory>
#include <vector>
#include "Algorithm.hpp"
#include "Evaluation.hpp"
#include "Solution.hpp"

/**
 * @class GeneticAlgorithm
 * @brief A class that represents a genetic algorithm for optimization problems.
 * 
 * The GeneticAlgorithm class is a subclass of the Algorithm class and implements a genetic algorithm
 * for solving optimization problems. It uses a population of solutions and applies genetic operators
 * such as mutation and recombination to evolve the population over multiple generations.
 * 
 * The genetic algorithm requires an evaluation function to assess the fitness of each solution in the population.
 * It also allows customization of various parameters such as population size, maximum number of generations,
 * mutation rate, crossover rate, mean, and standard deviation.
 */
class GeneticAlgorithm : public Algorithm {
protected:
    std::shared_ptr<Evaluation> eval;
    std::vector<float> populationFitness;
    size_t populationSize;
    size_t maxGenerations;
    float mutationRate;
    float crossoverRate;
    float mean;
    float std;

    virtual ~GeneticAlgorithm();

public:
    /**
     * @brief Gets the fitness values of the current population.
     * 
     * @return std::vector<float> The fitness values of the current population.
     */
    std::vector<float> getPopulationFitness() const;

    /**
     * @brief Sets the fitness values of the current population.
     * 
     * @param fitness The fitness values to set.
     * @return std::vector<float> The updated fitness values.
     */
    std::vector<float> setPopulationFitness(std::vector<float> fitness);

    /**
     * @brief Initializes the population with random solutions.
     * 
     * @param numberOfWeights The number of weights in each solution.
     * @return std::vector<Solution> The initialized population.
     */
    std::vector<Solution> initialize_population(size_t numberOfWeights);

    /**
     * @brief Mutates the population by applying random changes to the solutions.
     * 
     * @param population The current population.
     * @return std::vector<Solution> The mutated population.
     */
    virtual std::vector<Solution> mutate_population(const std::vector<Solution>& population) = 0;

    /**
     * @brief Selects individuals from the population for reproduction.
     * 
     * This method needs to be implemented by derived classes.
     * 
     * @param population The current population.
     * @return std::vector<Solution> The selected individuals for reproduction.
     */
    virtual std::vector<Solution> select_for_reproduction(const std::vector<Solution>& population) = 0;

    /**
     * @brief Recombines the selected individuals to create a new population.
     * 
     * This method needs to be implemented by derived classes.
     * 
     * @param population The current population.
     * @return std::vector<Solution> The recombined population.
     */
    virtual std::vector<Solution> recombine_population(const std::vector<Solution>& population) = 0;

    /**
     * @brief Replaces the current population with the new population.
     * 
     * This method needs to be implemented by derived classes.
     * 
     * @param population The current population.
     * @param newPopulation The new population.
     * @param dataset The dataset used for evaluation.
     */
    virtual void replace_population(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) = 0;

    GeneticAlgorithm(std::shared_ptr<Evaluation> eval, size_t populationSize=50, size_t maxGenerations=15000,
                     float mutationRate=0.08f, float crossoverRate=0.68f, float mean=0.0f, float std=0.3f);

    /**
     * @brief Runs the genetic algorithm on the given dataset.
     * 
     * @param dataset The dataset to run the algorithm on.
     * @return EvaluatedSolution The best solution found by the algorithm.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
