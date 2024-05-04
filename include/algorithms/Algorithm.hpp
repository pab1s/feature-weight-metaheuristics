#pragma once

#include <vector>
#include <memory>
#include "data/DataSet.hpp"
#include "Solution.hpp"
#include "Evaluation.hpp"
#include "data/EvaluatedSolution.hpp"

/**
 * @brief The base class for all algorithms.
 * 
 * This class provides an interface for implementing different algorithms.
 * Subclasses must implement the `run` method to define the algorithm's behavior.
 */
class Algorithm {
protected:
    size_t evaluations = 0;
public:

    /**
     * @brief Gets the number of evaluations performed by the algorithm.
     * 
     * @return The number of evaluations performed by the algorithm.
     */
    inline size_t getEvaluations() const { return evaluations; }

    /**
     * @brief Increments the number of evaluations performed by the algorithm.
     */
    inline void clearEvaluations() { evaluations = 0; }

    /**
     * @brief Runs the algorithm on the given dataset.
     * 
     * @param dataset The dataset to run the algorithm on.
     * @return The solution obtained by the algorithm and its fitness value.
     */
    virtual EvaluatedSolution run(const DataSet& dataset) = 0;

    /**
     * @brief Virtual destructor for the Algorithm class.
     */
    virtual ~Algorithm() {}
};
