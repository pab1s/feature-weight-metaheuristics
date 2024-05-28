#pragma once

#include "algorithms/Algorithm.hpp"
#include "algorithms/SA.hpp"
#include <memory>

class ILSSA : public Algorithm {
private:
    std::shared_ptr<Evaluation> eval;
    std::shared_ptr<SA> sa;
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
     * @brief Constructor of the ILS_SA class.
     * 
     * @param eval Evaluation object used to evaluate solutions
     * @param sa Simulated Annealing object used to escape local optima
     * @param maxIterations Maximum number of iterations to run the algorithm
     * @param mutationLimit Maximum percentage of weights to mutate
     */
    ILSSA(std::shared_ptr<Evaluation> eval, std::shared_ptr<SA> sa, size_t maxIterations, float mutationLimit=0.25f);

    /**
     * @brief Runs the ILS_SA algorithm on the given dataset.
     * 
     * @param dataset Dataset to run the algorithm on
     * @return The solution obtained by the algorithm and its fitness value.
     */
    EvaluatedSolution run(const DataSet& dataset) override;
};