#include "algorithms/GGA_BLX.hpp"
#include "utils/RandomUtils.hpp"
#include <algorithm>
#include <unordered_set>

std::vector<Solution> GGABLX::mutate_population(const std::vector<Solution>& population) {
    size_t M = population.size();
    size_t n = population[0].weights.size();
    size_t expectedMutations = static_cast<size_t>(M * n * mutationRate);

    std::vector<Solution> mutated = population;

    for (size_t i = 0; i < expectedMutations; ++i) {
        size_t chromoIndex = RandomUtils::getRandomInt(0, M - 1);
        size_t geneIndex = RandomUtils::getRandomInt(0, n - 1);
        mutated[chromoIndex].weights[geneIndex] += RandomUtils::getRandomNormal(mean, std);
        mutated[chromoIndex].weights[geneIndex] = std::clamp(mutated[chromoIndex].weights[geneIndex], 0.0f, 1.0f);
    }

    return mutated;
}

std::vector<Solution> GGABLX::select_for_reproduction(const std::vector<Solution>& population) {
    std::vector<Solution> selected;
    selected.reserve(population.size());

    for (size_t i = 0; i < population.size(); ++i) {
        size_t bestIdx = RandomUtils::getRandomInt(0, population.size() - 1);
        for (int j = 0; j < 2; ++j) {
            size_t idx = RandomUtils::getRandomInt(0, population.size() - 1);
            if (populationFitness[idx] > populationFitness[bestIdx]) {
                bestIdx = idx;
            }
        }
        selected.push_back(population[bestIdx]);
    }

    return selected;
}

std::vector<Solution> GGABLX::recombine_population(const std::vector<Solution>& population) {
    size_t M = population.size();
    std::vector<Solution> offspring(M, Solution(population[0].size()));

    for (size_t i = 0; i < M; i += 2) {
        const auto& parent1 = population[i];
        const auto& parent2 = population[i + 1];
        Solution child1(parent1.weights.size()), child2(parent2.weights.size());

        if (RandomUtils::getRandomFloat(0.0, 1.0) < crossoverRate) {
            // BLX-alpha crossover
            for (size_t j = 0; j < parent1.weights.size(); ++j) {
                float Cmax = std::max(parent1.weights[j], parent2.weights[j]);
                float Cmin = std::min(parent1.weights[j], parent2.weights[j]);
                float I = Cmax - Cmin;

                float lowerBound = Cmin - alpha * I;
                float upperBound = Cmax + alpha * I;

                child1.weights[j] = RandomUtils::getRandomFloat(lowerBound, upperBound);
                child2.weights[j] = RandomUtils::getRandomFloat(lowerBound, upperBound);
            }
        } else {
            child1 = parent1;
            child2 = parent2;
        }

        offspring.push_back(child1);
        offspring.push_back(child2);
    }

    return offspring;
}

void GGABLX::replace_population(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) {
    std::vector<float> newPopulationFitness(newPopulation.size());
    for (size_t i = 0; i < newPopulation.size(); ++i) {
        newPopulationFitness[i] = eval->calculateFitness(dataset, newPopulation[i]);
        ++evaluations;
    }

    // Identify the best fitness in the old population
    float bestOldFitness = *std::max_element(populationFitness.begin(), populationFitness.end());
    size_t bestOldIndex = std::distance(populationFitness.begin(), std::max_element(populationFitness.begin(), populationFitness.end()));

    // Identify the best and worst fitness in the new population
    auto worstNewIndex = std::distance(newPopulationFitness.begin(), std::min_element(newPopulationFitness.begin(), newPopulationFitness.end()));
    float bestNewFitness = *std::max_element(newPopulationFitness.begin(), newPopulationFitness.end());

    if (bestOldFitness > bestNewFitness) {
        newPopulation[worstNewIndex] = population[bestOldIndex];
        newPopulationFitness[worstNewIndex] = bestOldFitness;
    }

    population = std::move(newPopulation);
    populationFitness = std::move(newPopulationFitness);
}
