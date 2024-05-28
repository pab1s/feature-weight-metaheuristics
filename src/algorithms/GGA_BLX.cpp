#include "algorithms/GGA_BLX.hpp"
#include "utils/RandomUtils.hpp"
#include <algorithm>

std::vector<Solution> GGABLX::select_for_reproduction(const std::vector<Solution>& population) {
    return tournament_selection(population, population.size(), 3);
}

std::vector<Solution> GGABLX::mutate_population(const std::vector<Solution>& population) {
    size_t populationSize = population.size();
    size_t numWeights = population[0].weights.size();
    size_t expectedMutations = static_cast<size_t>(populationSize * numWeights * mutationRate);

    std::vector<Solution> mutated = population;

    for (size_t i = 0; i < expectedMutations; ++i) {
        size_t chromoIndex = RandomUtils::getRandomInt(0, populationSize - 1);
        size_t geneIndex = RandomUtils::getRandomInt(0, numWeights - 1);
        mutated[chromoIndex].weights[geneIndex] += RandomUtils::getRandomNormal(mean, std);
        mutated[chromoIndex].weights[geneIndex] = std::clamp(mutated[chromoIndex].weights[geneIndex], 0.0f, 1.0f);
    }

    return mutated;
}

std::vector<Solution> GGABLX::recombine_population(const std::vector<Solution>& population) {
    size_t populationSize = population.size();
    std::vector<Solution> offspring;

    for (size_t i = 0; i < populationSize; i += 2) {
        const auto& parent1 = population[i];
        const auto& parent2 = population[i + 1];
        std::vector<Solution> children(2, Solution(parent1.size()));

        if (RandomUtils::getRandomFloat(0.0, 1.0) < crossoverRate) {
            children = blend_crossover(parent1, parent2, 0.3);
        } else {
            children[0] = parent1;
            children[1] = parent2;
        }

        offspring.insert(offspring.end(), children.begin(), children.end());
    }

    return offspring;
}

void GGABLX::replace_population(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) {
    elitist_replacement(population, newPopulation, dataset);
}
