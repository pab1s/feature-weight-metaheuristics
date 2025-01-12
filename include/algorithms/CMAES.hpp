#pragma once

#include "algorithms/Algorithm.hpp"
#include <Eigen/Dense>

class CMAES : public Algorithm {
private:
    std::shared_ptr<Evaluation> eval;
    size_t maxEvaluations;
    size_t lambda;                // Population size
    size_t mu;                    // Number of parents
    std::vector<float> weights;   // Recombination weights
    float mueff;                  // Variance-effectiveness of sum w_i x_i
    float cc;                     // Time constant for cumulation for C
    float cs;                     // Time constant for cumulation for sigma control
    float c1;                     // Learning rate for rank-one update
    float cmu;                    // Learning rate for rank-mu update
    float damps;                  // Damping for sigma
    float chiN;                   // Expectation of ||N(0,I)||

    void initializeParameters(size_t dimension);
    std::vector<Solution> generatePopulation(const Eigen::VectorXf& mean, 
                                           float sigma,
                                           const Eigen::MatrixXf& C,
                                           size_t dimension);
    std::vector<size_t> getSortedIndices(const std::vector<float>& fitness);

public:
    CMAES(std::shared_ptr<Evaluation> eval, 
          size_t maxEvaluations,
          size_t lambda = 0);  // 0 means default (4 + floor(3*ln(N)))
    
    virtual EvaluatedSolution run(const DataSet& dataset) override;
}; 