#pragma once
#include "algorithms/Algorithm.hpp"

/**
 * @brief The Relief class represents the Relief algorithm.
 * 
 * This class inherits from the Algorithm class and implements the run method.
 * The Relief algorithm is a feature selection algorithm that aims to find the most
 * relevant features in a dataset.
 */
class Relief : public Algorithm {
private:
    std::shared_ptr<Evaluation> eval;
    
public:
    /**
     * @brief Constructor.
     * 
     * @param eval The evaluation object to use.
     */
    Relief(std::shared_ptr<Evaluation> eval);

    /**
     * @brief Runs the Relief algorithm on the given dataset.
     * 
     * @param dataset The dataset on which to run the algorithm.
     * @return The solution obtained by the algorithm.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
