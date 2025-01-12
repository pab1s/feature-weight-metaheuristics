#pragma once
#include "algorithms/Algorithm.hpp"
#include "algorithms/LocalSearch.hpp"
#include "algorithms/BestLocalSearch.hpp"
#include "algorithms/Relief.hpp"
#include "algorithms/MA.hpp"
#include "algorithms/MA-restart.hpp"
#include "algorithms/BMB.hpp"
#include "algorithms/CMAES.hpp"
#include "algorithms/ILS_SA.hpp"
#include "algorithms/SA.hpp"
#include "algorithms/GGA_AC.hpp"
#include "algorithms/SGA_AC.hpp"

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
