#include "algorithms/MA.hpp"
#include "utils/RandomUtils.hpp"
#include <algorithm>
#include <numeric>

MemeticAlgorithm::MemeticAlgorithm(std::shared_ptr<Evaluation> eval, std::shared_ptr<GeneticAlgorithm> ga, std::shared_ptr<LocalSearch> ls,
                                   size_t maxGenerations, size_t optimizationFrequency, float elitismRate, float selectionRate)
    : eval(eval), ga(ga), ls(ls), maxGenerations(maxGenerations), optimizationFrequency(optimizationFrequency),
      elitismRate(elitismRate), selectionRate(selectionRate) {}

MemeticAlgorithm::~MemeticAlgorithm() {}

EvaluatedSolution MemeticAlgorithm::run(const DataSet& dataset) {
    size_t generation = 0;
    std::vector<Solution> population = ga->initialize_population(dataset.getNumFeatures());
    populationFitness = eval->evaluatePopulation(population, dataset);
    optimize_population(population, dataset);
    ga->setPopulationFitness(populationFitness);
    eval->insertFitnessRecord(*std::max_element(populationFitness.begin(), populationFitness.end()));

    while (generation < maxGenerations) {
        std::vector<Solution> selected = ga->select_for_reproduction(population);
        std::vector<Solution> offspring = ga->recombine_population(selected);
        offspring = ga->mutate_population(offspring);
        
        if (generation % optimizationFrequency == 0) {
            optimize_population(population, dataset);
        }

        ga->replace_population(population, offspring, dataset);

        populationFitness = ga->getPopulationFitness();
        eval->insertFitnessRecord(*std::max_element(populationFitness.begin(), populationFitness.end()));
        generation += ls->getEvaluations() + ga->getEvaluations();
        ls->clearEvaluations();
        ga->clearEvaluations();
    }

    // Find the best solution in the population
    size_t bestIndex = std::distance(populationFitness.begin(), std::max_element(populationFitness.begin(), populationFitness.end()));
    return EvaluatedSolution{population[bestIndex], populationFitness[bestIndex]};
}

void MemeticAlgorithm::optimize_population(std::vector<Solution>& population, const DataSet& dataset) {
    size_t numToSelect = static_cast<size_t>(selectionRate * population.size());
    size_t numElitist = static_cast<size_t>(elitismRate * numToSelect);
    size_t numRandom = numToSelect - numElitist;

    // Sort indices based on fitness in descending order
    std::vector<size_t> indices(population.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
        return populationFitness[a] > populationFitness[b];
    });

    // Insert the indices of the elitist individuals and choose the rest randomly
    std::vector<size_t> selectedIndices;
    selectedIndices.insert(selectedIndices.end(), indices.begin(), indices.begin() + numElitist);
    indices = RandomUtils::shuffleIntArray(indices, numElitist);
    selectedIndices.insert(selectedIndices.end(), indices.begin() + numElitist, indices.begin() + numElitist + numRandom);

    // Optimize the selected individuals using local search
    for (size_t index : selectedIndices) {
        EvaluatedSolution optimized = ls->run(population[index], dataset);
        population[index] = optimized.solution;
        populationFitness[index] = optimized.fitness;
    }
}
