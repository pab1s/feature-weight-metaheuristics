#include "algorithms/GA.hpp"
#include "Evaluation.hpp"
#include <algorithm>
#include <cmath>
#include "utils/RandomUtils.hpp"

GeneticAlgorithm::~GeneticAlgorithm() {}

GeneticAlgorithm::GeneticAlgorithm(std::shared_ptr<Evaluation> eval,
                                   size_t populationSize,
                                   size_t maxGenerations,
                                   float mutationRate,
                                   float crossoverRate,
                                   float mean,
                                   float std)
    : eval(eval), populationSize(populationSize), maxGenerations(maxGenerations),
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

EvaluatedSolution GeneticAlgorithm::run(const DataSet& dataset) {
    auto population = initialize_population(dataset.getNumFeatures());
    populationFitness = eval->evaluatePopulation(population, dataset);

    while (evaluations < maxGenerations) {
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
