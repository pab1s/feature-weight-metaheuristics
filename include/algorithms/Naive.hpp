#pragma once

#include "algorithms/Algorithm.hpp"

class Naive : public Algorithm {
private:
    std::shared_ptr<Evaluation> eval;

public:
    /**
     * Constructor.
     *
     * @param eval The evaluation object to use.
     */
    Naive(std::shared_ptr<Evaluation> eval);

    /**
     * Runs the algorithm.
     *
     * @param dataset The dataset to run the algorithm on.
     * @return The trivial solution with all features weights set to 1.0.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
