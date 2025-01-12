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
    
    // Initialize and evaluate initial population
    std::vector<Solution> population = ga->initialize_population(dataset.getNumFeatures());
    populationFitness = ga->evaluatePopulation(population, dataset);
    evaluations += ga->getEvaluations();
    ga->clearEvaluations();
    ga->setPopulationFitness(populationFitness);
    
    // Initial optimization
    optimize_population(population, dataset);
    evaluations += ls->getEvaluations();
    ls->clearEvaluations();
    
    // Re-evaluate population after optimization and update fitness
    populationFitness = ga->evaluatePopulation(population, dataset);
    evaluations += ga->getEvaluations();
    ga->clearEvaluations();
    ga->setPopulationFitness(populationFitness);
    
    eval->insertFitnessRecord(*std::max_element(populationFitness.begin(), populationFitness.end()));

    while (evaluations < maxEvaluations) {
        meanVariance = 0;
        for (size_t i = 0; i < static_cast<size_t>(dataset.getNumFeatures()); ++i) {
            meanVariance += calculateVariance(population, i);
        }
        meanVariance /= dataset.getNumFeatures();

        if (meanVariance < restartFrequency) {
            std::cout << "Restarting population: " << generation << std::endl;
            // Store best solution before restart
            auto bestIt = std::max_element(populationFitness.begin(), populationFitness.end());
            Solution bestSolution = population[std::distance(populationFitness.begin(), bestIt)];
            
            // Reinitialize population and preserve best solution
            population = ga->initialize_population(dataset.getNumFeatures());
            population[0] = bestSolution;
            
            // Evaluate new population
            populationFitness = ga->evaluatePopulation(population, dataset);
            evaluations += ga->getEvaluations();
            ga->clearEvaluations();
            ga->setPopulationFitness(populationFitness);
            meanVariance = 0;
            continue;  // Skip to next iteration to avoid duplicate evolution step
        }

        // Evolution step
        std::vector<Solution> selected = ga->select_for_reproduction(population);
        std::vector<Solution> offspring = ga->recombine_population(selected);
        offspring = ga->mutate_population(offspring);
        
        // Evaluate offspring and perform replacement
        std::vector<float> offspringFitness = ga->evaluatePopulation(offspring, dataset);
        evaluations += ga->getEvaluations();
        ga->clearEvaluations();
        
        // Replace population and update fitness
        ga->replace_population(population, offspring, dataset);
        populationFitness = ga->getPopulationFitness();
        
        // Local search step
        if (generation % optimizationFrequency == 0) {
            optimize_population(population, dataset);
            evaluations += ls->getEvaluations();
            ls->clearEvaluations();
            
            // Re-evaluate population after optimization
            populationFitness = ga->evaluatePopulation(population, dataset);
            evaluations += ga->getEvaluations();
            ga->clearEvaluations();
            ga->setPopulationFitness(populationFitness);
        }

        eval->insertFitnessRecord(*std::max_element(populationFitness.begin(), populationFitness.end()));
        ++generation;
    }

    auto bestIt = std::max_element(populationFitness.begin(), populationFitness.end());
    size_t bestIndex = std::distance(populationFitness.begin(), bestIt);
    return EvaluatedSolution{population[bestIndex], populationFitness[bestIndex]};
}