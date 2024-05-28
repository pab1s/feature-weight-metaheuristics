#include "algorithms/SGA_BLX.hpp"
#include "utils/RandomUtils.hpp"
#include <algorithm>

std::vector<Solution> SGABLX::select_for_reproduction(const std::vector<Solution>& population) {
    return tournament_selection(population, 2, 3);
}

std::vector<Solution> SGABLX::mutate_population(const std::vector<Solution>& population) {
    std::vector<Solution> mutated;
    mutated.reserve(population.size());

    for (const auto& solution : population) {
        Solution newSolution(solution.size());
        if (RandomUtils::getRandomFloat(0.0, 1.0) < mutationRate) {
            size_t geneIndex = RandomUtils::getRandomInt(0, solution.size() - 1);
            newSolution.weights[geneIndex] += RandomUtils::getRandomNormal(mean, std);
            newSolution.weights[geneIndex] = std::clamp(newSolution.weights[geneIndex], 0.0f, 1.0f);
        } else {
            newSolution = solution;
        }
        mutated.push_back(newSolution);
    }

    return mutated;
}

std::vector<Solution> SGABLX::recombine_population(const std::vector<Solution>& population) {
    return blend_crossover(population[0], population[1], 0.3);
}

void SGABLX::replace_population(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) {
    competitive_replacement(population, newPopulation, dataset);
}