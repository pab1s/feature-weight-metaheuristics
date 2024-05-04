#pragma once

#include "algorithms/LocalSearch.hpp"
#include <memory>

class BestLocalSearch : public LocalSearch {
public:
    /**
     * @brief Construct a new Best Local Search object
     * 
     * @param eval Evaluation object
     * @param maxNeighbors Maximum number of neighbors to consider
     * @param maxEvaluations Maximum number of evaluations
     * @param mean Mean of the normal distribution
     * @param variance Variance of the normal distribution
     */
    BestLocalSearch(std::shared_ptr<Evaluation> eval, size_t maxNeighbors=20, 
        size_t maxEvaluations=15000, float mean=0.0f, float variance=0.3f);

    /**
     * @brief Run the best local search algorithm on the given dataset.
     * 
     * @param dataset The dataset on which the algorithm will be run.
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
