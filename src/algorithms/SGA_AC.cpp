#include "algorithms/SGA_AC.hpp"
#include "utils/RandomUtils.hpp"
#include <algorithm>
#include <unordered_set>
#include <array>

std::vector<Solution> SGAAC::mutate_population(const std::vector<Solution>& population) {
    std::vector<Solution> mutated;
    mutated.reserve(population.size());

    for (const auto& solution : population) {
        Solution newSolution(solution.weights.size());
        if (RandomUtils::getRandomFloat(0.0, 1.0) < mutationRate) {
            size_t geneIndex = RandomUtils::getRandomInt(0, solution.weights.size() - 1);
            newSolution.weights[geneIndex] += RandomUtils::getRandomNormal(mean, std);
            newSolution.weights[geneIndex] = std::clamp(newSolution.weights[geneIndex], 0.0f, 1.0f);
        } else {
            newSolution = solution;
        }
        mutated.push_back(newSolution);
    }

    return mutated;
}

std::vector<Solution> SGAAC::select_for_reproduction(const std::vector<Solution>& population) {
    std::vector<Solution> selected;

    for (int i = 0; i < 2; ++i) {  // Only select two parents
        size_t bestIdx = RandomUtils::getRandomInt(0, population.size() - 1);
        for (int j = 0; j < 2; ++j) {  // Tournament of 3
            size_t idx = RandomUtils::getRandomInt(0, population.size() - 1);
            if (populationFitness[idx] > populationFitness[bestIdx]) {
                bestIdx = idx;
            }
        }
        selected.push_back(population[bestIdx]);
    }

    return selected;
}

std::vector<Solution> SGAAC::recombine_population(const std::vector<Solution>& population) {
    // Assuming population always has 2 parents
    const auto& parent1 = population[0];
    const auto& parent2 = population[1];
    Solution child1(parent1.size()), child2(parent2.size());

    // Arithmetic crossover
    for (size_t j = 0; j < parent1.weights.size(); ++j) {
        float alpha = RandomUtils::getRandomFloat(0.0, 1.0);
        child1.weights[j] = alpha * parent1.weights[j] + (1 - alpha) * parent2.weights[j];
        child2.weights[j] = (1 - alpha) * parent1.weights[j] + alpha * parent2.weights[j];
    }

    return {child1, child2};
}

void SGAAC::replace_population(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) {
    std::vector<float> newPopulationFitness(newPopulation.size());
    for (size_t i = 0; i < newPopulation.size(); ++i) {
        newPopulationFitness[i] = eval->calculateFitness(dataset, newPopulation[i]);
        ++evaluations;
    }

    // Find two best new solutions
    std::array<size_t, 2> bestNewIdx = {0, 1};
    if (newPopulationFitness[bestNewIdx[0]] < newPopulationFitness[bestNewIdx[1]]) {
        std::swap(bestNewIdx[0], bestNewIdx[1]);
    }
    for (size_t i = 2; i < newPopulation.size(); ++i) {
        if (newPopulationFitness[i] > newPopulationFitness[bestNewIdx[1]]) {
            bestNewIdx[1] = i;
            if (newPopulationFitness[bestNewIdx[1]] > newPopulationFitness[bestNewIdx[0]]) {
                std::swap(bestNewIdx[0], bestNewIdx[1]);
            }
        }
    }

    // Find two worst old solutions
    std::array<size_t, 2> worstOldIdx = {0, 1};
    if (populationFitness[worstOldIdx[0]] > populationFitness[worstOldIdx[1]]) {
        std::swap(worstOldIdx[0], worstOldIdx[1]);
    }
    for (size_t i = 2; i < population.size(); ++i) {
        if (populationFitness[i] < populationFitness[worstOldIdx[1]]) {
            worstOldIdx[1] = i;
            if (populationFitness[worstOldIdx[1]] < populationFitness[worstOldIdx[0]]) {
                std::swap(worstOldIdx[0], worstOldIdx[1]);
            }
        }
    }

    // Select the top two from the four candidates (two best new and two worst old)
    std::vector<std::pair<float, Solution>> candidates = {
        {newPopulationFitness[bestNewIdx[0]], newPopulation[bestNewIdx[0]]},
        {newPopulationFitness[bestNewIdx[1]], newPopulation[bestNewIdx[1]]},
        {populationFitness[worstOldIdx[0]], population[worstOldIdx[0]]},
        {populationFitness[worstOldIdx[1]], population[worstOldIdx[1]]}
    };

    // Sort and select the top two
    std::sort(candidates.begin(), candidates.end(), [](const std::pair<float, Solution>& a, const std::pair<float, Solution>& b) {
        return a.first > b.first; // Sorting by fitness, descending
    });

    // Replace the two worst old solutions with the selected top two
    population[worstOldIdx[0]] = candidates[0].second;
    population[worstOldIdx[1]] = candidates[1].second;
    populationFitness[worstOldIdx[0]] = candidates[0].first;
    populationFitness[worstOldIdx[1]] = candidates[1].first;
}
