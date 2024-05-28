#pragma once

#include "algorithms/Algorithm.hpp"
#include "algorithms/LocalSearch.hpp"

#include <memory>

class BMB : public Algorithm {
private:
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<LocalSearch> ls;
    size_t maxIterations;

public:
    /**
     * @brief Construct a new BMB object.
     * 
     * @param eval Evaluation object to use
     * @param ls Local search object to use
     * @param maxIterations Maximum number of iterations to run the algorithm
     */
    BMB(std::shared_ptr<Evaluation> eval, std::shared_ptr<LocalSearch> ls, size_t maxIterations);

    /**
     * @brief Runs the BMB algorithm on the given dataset.
     * 
     * @param dataset Dataset to run the algorithm on
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
