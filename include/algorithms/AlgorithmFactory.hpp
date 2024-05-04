#pragma once
#include "algorithms/Algorithm.hpp"

class AlgorithmFactory {
   public:
    /**
     * Creates an algorithm based on the given name and parameters.
     *
     * @param name The name of the algorithm to create.
     * @param parameters The parameters to use when creating the algorithm.
     * @return The created algorithm.
     */
    static std::unique_ptr<Algorithm> createAlgorithm(
        const std::string& name, const std::map<std::string, float>& parameters,
        std::shared_ptr<Evaluation> eval);
};
