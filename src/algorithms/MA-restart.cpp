#include "algorithms/MA-restart.hpp"

#include <algorithm>

float MemeticRestart::calculateVariance(const std::vector<Solution>& population, size_t i) {
    float mean = 0;
    for (size_t j = 0; j < population.size(); ++j) {
        mean += population[j].getWeight(i);
    }
    mean /= population.size();

    float variance = 0;
    for (size_t j = 0; j < population.size(); ++j) {
        variance += (population[j].getWeight(i) - mean) * (population[j].getWeight(i) - mean);
    }
    variance /= population.size();
    return variance;
}

EvaluatedSolution MemeticRestart::run(const DataSet& dataset) {
    size_t evaluations = 0;
    size_t generation = 0;
    float meanVariance = 0;
    std::vector<Solution> population = ga->initialize_population(dataset.getNumFeatures());
    populationFitness = ga->evaluatePopulation(population, dataset);
    optimize_population(population, dataset);
    ga->setPopulationFitness(populationFitness);
    eval->insertFitnessRecord(*std::max_element(populationFitness.begin(), populationFitness.end()));

    while (evaluations < maxEvaluations) {
        for (size_t i = 0; i < static_cast<size_t>(dataset.getNumFeatures()); ++i) {
            meanVariance += calculateVariance(population, i);
            meanVariance /= dataset.getNumFeatures();
        }

        if (meanVariance < restartFrequency) {
            std::cout << "Restarting population: " << generation << std::endl;
            Solution bestSolution = population[std::distance(populationFitness.begin(), std::max_element(populationFitness.begin(), populationFitness.end()))];
            population = ga->initialize_population(dataset.getNumFeatures());
            population[0] = bestSolution;
            populationFitness = ga->evaluatePopulation(population, dataset);
            ga->setPopulationFitness(populationFitness);
            evaluations += ga->getEvaluations();
            ga->clearEvaluations();
            meanVariance = 0;
        }

        std::vector<Solution> selected = ga->select_for_reproduction(population);
        std::vector<Solution> offspring = ga->recombine_population(selected);
        offspring = ga->mutate_population(offspring);
        
        if (generation % optimizationFrequency == 0) {
            optimize_population(population, dataset);
        }

        ga->replace_population(population, offspring, dataset);

        populationFitness = ga->getPopulationFitness();
        eval->insertFitnessRecord(*std::max_element(populationFitness.begin(), populationFitness.end()));
        evaluations += ls->getEvaluations() + ga->getEvaluations();
        ls->clearEvaluations();
        ga->clearEvaluations();
        ++generation;
    }

    // Find the best solution in the population
    size_t bestIndex = std::distance(populationFitness.begin(), std::max_element(populationFitness.begin(), populationFitness.end()));
    return EvaluatedSolution{population[bestIndex], populationFitness[bestIndex]};
}