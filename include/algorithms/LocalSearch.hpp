#pragma once
#include "algorithms/Algorithm.hpp"
#include "Evaluation.hpp"

/**
 * @brief The LocalSearch class represents a local search algorithm.
 * 
 * This class inherits from the Algorithm class and implements the run method.
 * The run method takes a dataset and an alpha value as input and returns a Solution object.
 */
/**
 * @file LocalSearch.hpp
 * @brief Contains the declaration of the LocalSearch class.
 */

/**
 * @class LocalSearch
 * @brief Represents a local search algorithm.
 * 
 * The LocalSearch class is a subclass of the Algorithm class and implements a local search algorithm.
 * It generates neighbor solutions by adding random values to the weights of the current solution.
 * The algorithm runs on a given dataset and returns the best solution found along with its fitness value.
 */
class LocalSearch : public Algorithm {
protected:
    std::shared_ptr<Evaluation> eval; /**< The evaluation object used to evaluate solutions. */
    size_t maxNeighbors; /**< The maximum number of neighbor solutions to generate. */
    size_t maxEvaluations; /**< The maximum number of evaluations to perform. */
    float mean; /**< The mean value used for generating random values. */
    float std; /**< The standard deviation used for generating random values. */
    
public:
    /**
     * @brief Constructor of the LocalSearch class.
     * 
     * @param eval The evaluation object used to evaluate solutions.
     * @param maxNeighbors The maximum number of neighbor solutions to generate. Default is 20.
     * @param maxEvaluations The maximum number of evaluations to perform. Default is 15000.
     * @param mean The mean value used for generating random values. Default is 0.0f.
     * @param std The standard deviation used for generating random values. Default is 0.3f.
     */
    LocalSearch(std::shared_ptr<Evaluation> eval, size_t maxNeighbors=20, 
        size_t maxEvaluations=15000, float mean=0.0f, float std=0.3f);

    /**
     * @brief Generates a neighbor solution by adding a random value to the weight at the given index.
     * 
     * @param solution Solution from which the neighbor will be generated.
     * @param index Index of the weight that will be modified.
     * @return Solution object representing the neighbor solution.
     */
    Solution generateNeighbor(const Solution& solution, size_t index);

    /**
     * @brief Runs the local search algorithm on the given solution and dataset.
     * 
     * @param solution The initial solution to start the algorithm from.
     * @param dataset The dataset on which the algorithm will be run.
     * @return The solution obtained by the algorithm and its fitness value.
     */
    virtual EvaluatedSolution run(Solution solution, const DataSet& dataset);

    /**
     * @brief Runs the local search algorithm on the given dataset.
     * 
     * @param dataset The dataset on which the algorithm will be run.
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution runTrayectories(Solution solution, const DataSet& dataset);

    /**
     * @brief Runs the local search algorithm on the given dataset.
     * 
     * @param dataset The dataset on which the algorithm will be run.
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
