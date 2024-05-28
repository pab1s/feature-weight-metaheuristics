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
    size_t maxEvaluations;
    float mutationRate;
    float crossoverRate;
    float mean;
    float std;

    virtual ~GeneticAlgorithm();

    /**
     * @brief Performs tournament selection on the population.
     * 
     * The tournament selection operator selects individuals for reproduction by comparing
     * the fitness values of randomly chosen individuals in the population.
     * 
     * @param population The current population.
     * @param newPopulationSize The number of individuals to select.
     * @param tournamentSize The size of the tournament.
     * @return std::vector<Solution> The selected individuals.
     */
    std::vector<Solution> tournament_selection(const std::vector<Solution>& population, size_t newPopulationSize=2, size_t tournamentSize=3);

    /**
     * @brief Performs blend crossover between two parents.
     * 
     * The blend crossover operator creates two children by combining the genes of two parents
     * using a weighted average within a specified range.
     * 
     * @param parent1 The first parent.
     * @param parent2 The second parent.
     * @param alpha The alpha value for the blend crossover.
     * @return std::vector<Solution> The resulting children.
     */
    std::vector<Solution> blend_crossover(const Solution& parent1, const Solution& parent2, float alpha=0.3f);

    /**
     * @brief Performs arithmetic crossover between two parents.
     * 
     * The arithmetic crossover operator creates two children by combining the genes of two parents
     * using a weighted average.
     * 
     * @param parent1 The first parent.
     * @param parent2 The second parent.
     * @return std::vector<Solution> The resulting children.
     */
    std::vector<Solution> arithmetic_crossover(const Solution& parent1, const Solution& parent2);

    /**
     * @brief Performs competitive replacement on the population.
     * 
     * The competitive replacement strategy replaces the worst individuals in the population with the best individuals
     * from the new population.
     * 
     * @param population The current population.
     * @param newPopulation The new population.
     * @param dataset The dataset used for evaluation.
     */
    void competitive_replacement(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset);
    
    /**
     * @brief Performs elitist replacement on the population.
     * 
     * The elitist replacement strategy replaces the worst individual in the population with the best individual
     * from the new population if the best individual in the new population is better than the best individual in the old population.
     * 
     * @param population The current population.
     * @param newPopulation The new population.
     * @param dataset The dataset used for evaluation.
     */
    void elitist_replacement(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset);

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
     * @brief Selects individuals from the population for reproduction by using
     * tournament selection with a tournament size of 3.
     * 
     * @param population The current population.
     * @return std::vector<Solution> The selected individuals for reproduction.
     */
    virtual std::vector<Solution> select_for_reproduction(const std::vector<Solution>& population) = 0;

    /**
     * @brief Mutates the population by applying random changes to the solutions.
     * 
     * @param population The current population.
     * @return std::vector<Solution> The mutated population.
     */
    virtual std::vector<Solution> mutate_population(const std::vector<Solution>& population) = 0;

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

    /**
     * @brief Evaluates the fitness of the population using the evaluation function.
     * 
     * @param population The population to evaluate.
     * @param dataset The dataset used for evaluation.
     * @return std::vector<float> The fitness values of the population.
     */
    std::vector<float> evaluatePopulation(const std::vector<Solution>& population, const DataSet& dataset);

    /**
     * @brief Constructs a new GeneticAlgorithm object with the specified parameters.
     * 
     * @param eval The evaluation function to use.
     * @param populationSize The size of the population.
     * @param maxEvaluations The maximum number of evaluations.
     * @param mutationRate The mutation rate.
     * @param crossoverRate The crossover rate.
     * @param mean The mean for the mutation operator.
     * @param std The standard deviation for the mutation operator.
     */
    GeneticAlgorithm(std::shared_ptr<Evaluation> eval, size_t populationSize=50, size_t maxEvaluations=15000,
                     float mutationRate=0.08f, float crossoverRate=0.68f, float mean=0.0f, float std=0.3f);

    /**
     * @brief Runs the genetic algorithm on the given dataset.
     * 
     * @param dataset The dataset to run the algorithm on.
     * @return EvaluatedSolution The best solution found by the algorithm.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
