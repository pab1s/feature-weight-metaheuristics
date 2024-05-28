#include "algorithms/GA.hpp"
#include "Evaluation.hpp"
#include <algorithm>
#include <cmath>
#include "utils/RandomUtils.hpp"

GeneticAlgorithm::~GeneticAlgorithm() {}

GeneticAlgorithm::GeneticAlgorithm(std::shared_ptr<Evaluation> eval,
                                   size_t populationSize,
                                   size_t maxEvaluations,
                                   float mutationRate,
                                   float crossoverRate,
                                   float mean,
                                   float std)
    : eval(eval), populationSize(populationSize), maxEvaluations(maxEvaluations),
      mutationRate(mutationRate), crossoverRate(crossoverRate), mean(mean), std(std) {}

std::vector<float> GeneticAlgorithm::getPopulationFitness() const {
    return populationFitness;
}

std::vector<float> GeneticAlgorithm::setPopulationFitness(std::vector<float> fitness) {
    populationFitness = fitness;
    return populationFitness;
}

std::vector<Solution> GeneticAlgorithm::initialize_population(size_t numberOfWeights) {
    std::vector<Solution> population(populationSize, Solution(numberOfWeights));
    return population;
}

std::vector<Solution> GeneticAlgorithm::tournament_selection(const std::vector<Solution>& population, size_t newPopulationSize, size_t tournamentSize) {
    std::vector<Solution> selected;

    for (size_t i = 0; i < newPopulationSize; ++i) {
        size_t bestIdx = RandomUtils::getRandomInt(0, population.size() - 1);
        for (size_t j = 0; j < tournamentSize - 1; ++j) {
            size_t idx = RandomUtils::getRandomInt(0, population.size() - 1);
            if (populationFitness[idx] > populationFitness[bestIdx]) {
                bestIdx = idx;
            }
        }
        selected.push_back(population[bestIdx]);
    }

    return selected;
}

std::vector<float> GeneticAlgorithm::evaluatePopulation(const std::vector<Solution>& population, const DataSet& dataset) {
    std::vector<float> fitness(population.size());
    for (size_t i = 0; i < population.size(); ++i) {
        fitness[i] = eval->calculateFitnessLeaveOneOut(dataset, population[i]);
        ++evaluations;
    }
    return fitness;
}

EvaluatedSolution GeneticAlgorithm::run(const DataSet& dataset) {
    auto population = initialize_population(dataset.getNumFeatures());
    populationFitness = evaluatePopulation(population, dataset);

    while (evaluations < maxEvaluations) {
        float bestFitness = *std::max_element(populationFitness.begin(), populationFitness.end());
        eval->insertFitnessRecord(bestFitness);
        auto selected = select_for_reproduction(population);
        auto recombined = recombine_population(selected);
        auto mutated = mutate_population(recombined);
        replace_population(population, mutated, dataset);
    }

    size_t bestIndex = std::distance(populationFitness.begin(), std::max_element(populationFitness.begin(), populationFitness.end()));

    return EvaluatedSolution{population[bestIndex], populationFitness[bestIndex]};
}

std::vector<Solution> GeneticAlgorithm::blend_crossover(const Solution& parent1, const Solution& parent2, float alpha) {
    Solution child1(parent1.size()), child2(parent2.size());

    // BLX-alpha crossover
    for (size_t j = 0; j < parent1.size(); ++j) {
        float Cmax = std::max(parent1.weights[j], parent2.weights[j]);
        float Cmin = std::min(parent1.weights[j], parent2.weights[j]);
        float I = (Cmax - Cmin > std::numeric_limits<float>::epsilon()) ? (Cmax - Cmin) : 0.0f;

        float lowerBound = Cmin - alpha * I;
        float upperBound = Cmax + alpha * I;

        child1.weights[j] = std::clamp(RandomUtils::getRandomFloat(lowerBound, upperBound), 0.0f, 1.0f);
        child2.weights[j] = std::clamp(RandomUtils::getRandomFloat(lowerBound, upperBound), 0.0f, 1.0f);
    }

    return {child1, child2};
}

std::vector<Solution> GeneticAlgorithm::arithmetic_crossover(const Solution& parent1, const Solution& parent2) {
    Solution child1(parent1.size()), child2(parent2.size());

    // Arithmetic crossover
    for (size_t j = 0; j < parent1.size(); ++j) {
        float alpha = RandomUtils::getRandomFloat(0.0, 1.0);
        child1.weights[j] = alpha * parent1.weights[j] + (1 - alpha) * parent2.weights[j];
        child2.weights[j] = (1 - alpha) * parent1.weights[j] + alpha * parent2.weights[j];
    }

    return {child1, child2};
}

void GeneticAlgorithm::competitive_replacement(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) {
    std::vector<float> newPopulationFitness(newPopulation.size());
    for (size_t i = 0; i < newPopulation.size(); ++i) {
        newPopulationFitness[i] = eval->calculateFitnessLeaveOneOut(dataset, newPopulation[i]);
        ++evaluations;
    }

    // Sort new population indices by descending fitness (best first)
    std::vector<size_t> sortedNewIdx(newPopulation.size());
    std::iota(sortedNewIdx.begin(), sortedNewIdx.end(), 0);
    std::sort(sortedNewIdx.begin(), sortedNewIdx.end(), [&](size_t i, size_t j) {
        return newPopulationFitness[i] > newPopulationFitness[j];
    });

    // Sort old population indices by ascending fitness (worst first)
    std::vector<size_t> sortedOldIdx(population.size());
    std::iota(sortedOldIdx.begin(), sortedOldIdx.end(), 0);
    std::sort(sortedOldIdx.begin(), sortedOldIdx.end(), [&](size_t i, size_t j) {
        return populationFitness[i] < populationFitness[j];
    });
    
    // Iterate over the new population starting from the worst
    for (size_t newIdx = 0; newIdx < newPopulation.size(); ++newIdx) {
        bool replaced = false;
        for (size_t oldIdx = 0; oldIdx < newPopulation.size() && !replaced; ++oldIdx) {
            if (newPopulationFitness[sortedNewIdx[newIdx]] > populationFitness[sortedOldIdx[oldIdx]]) {
                // Replace the old individual with the new one
                population[sortedOldIdx[oldIdx]] = newPopulation[sortedNewIdx[newIdx]];
                populationFitness[sortedOldIdx[oldIdx]] = newPopulationFitness[sortedNewIdx[newIdx]];
                replaced = true;
            }
        }
    }
}

void GeneticAlgorithm::elitist_replacement(std::vector<Solution>& population, std::vector<Solution>& newPopulation, const DataSet& dataset) {
    std::vector<float> newPopulationFitness(newPopulation.size());
    for (size_t i = 0; i < newPopulation.size(); ++i) {
        newPopulationFitness[i] = eval->calculateFitnessLeaveOneOut(dataset, newPopulation[i]);
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
