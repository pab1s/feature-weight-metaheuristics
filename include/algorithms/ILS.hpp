#pragma once

#include "algorithms/Algorithm.hpp"
#include "algorithms/LocalSearch.hpp"
#include <memory>

class ILS : public Algorithm {
private:
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<LocalSearch> ls;
    size_t maxIterations;
    float mutationLimit;

    /**
     * @brief Mutates the given solution.
     * 
     * @param solution Solution to mutate
     * @param t Number of weights to mutate
     * @return The mutated solution.
     */
    Solution mutate(const Solution& solution, size_t t);

public:
    /**
     * @brief Constructor of the ILS class.
     * 
     * @param eval Evaluation object used to evaluate solutions
     * @param ls Local search object used to improve solutions
     * @param maxIterations Maximum number of iterations to run the algorithm
     * @param mutationLimit Maximum percentage of weights to mutate
     */
    ILS(std::shared_ptr<Evaluation> eval, std::shared_ptr<LocalSearch> ls, size_t maxIterations, float mutationLimit=0.25f);

    /**
     * @brief Runs the ILS algorithm on the given dataset.
     * 
     * @param dataset Dataset to run the algorithm on
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};
