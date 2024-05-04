#include <memory>
#include <map>
#include <string>
#include "Evaluation.hpp"
#include "algorithms/AlgorithmFactory.hpp"
#include "algorithms/LocalSearch.hpp"
#include "algorithms/BestLocalSearch.hpp"
#include "algorithms/Relief.hpp"
#include "algorithms/Naive.hpp"
#include "algorithms/SGA_BLX.hpp"
#include "algorithms/SGA_AC.hpp"
#include "algorithms/GGA_BLX.hpp"
#include "algorithms/GGA_AC.hpp"
#include "algorithms/MA.hpp"

std::unique_ptr<Algorithm> AlgorithmFactory::createAlgorithm(
    const std::string& name, 
    const std::map<std::string, float>& parameters,
    std::shared_ptr<Evaluation> eval
) {
    if (name == "local-search" || name == "best-local-search") {
        size_t defaultMaxNeighbors = 20;
        size_t defaultMaxEvaluations = 15000;
        float defaultVariance = 0.3;

        size_t maxNeighbors = parameters.find("maxNeighbors") != parameters.end() ? static_cast<size_t>(parameters.at("maxNeighbors")) : defaultMaxNeighbors;
        size_t maxEvaluations = parameters.find("maxEvaluations") != parameters.end() ? static_cast<size_t>(parameters.at("maxEvaluations")) : defaultMaxEvaluations;
        float variance = parameters.find("variance") != parameters.end() ? parameters.at("variance") : defaultVariance;

        if (name == "best-local-search") {
            return std::make_unique<BestLocalSearch>(eval, maxNeighbors, maxEvaluations, variance);
        } else {
            return std::make_unique<LocalSearch>(eval, maxNeighbors, maxEvaluations, variance);
        }
    } else if (name == "relief") {
        return std::make_unique<Relief>(eval);
    } else if (name == "1nn") {
        return std::make_unique<Naive>(eval);
    } else if (name == "age-blx" || name == "age-ac" || name == "agg-blx" || name == "agg-ac") {
        size_t defaultMaxGenerations = 15000;
        size_t defaultPopulationSize = 50;
        float defaultCrossoverRate = 0.8;
        float defaultMutationRate = 0.1;

        size_t maxGenerations = parameters.find("maxGenerations") != parameters.end() ? static_cast<size_t>(parameters.at("maxGenerations")) : defaultMaxGenerations;
        size_t populationSize = parameters.find("populationSize") != parameters.end() ? static_cast<size_t>(parameters.at("populationSize")) : defaultPopulationSize;
        float crossoverRate = parameters.find("crossoverRate") != parameters.end() ? parameters.at("crossoverRate") : defaultCrossoverRate;
        float mutationRate = parameters.find("mutationRate") != parameters.end() ? parameters.at("mutationRate") : defaultMutationRate;

        return std::make_unique<SGABLX>(eval, populationSize, maxGenerations, crossoverRate, mutationRate);
    } else if (name == "memetic") {
        size_t defaultMaxGenerations = 15000;
        size_t defaultOptimizationFrequency = 10;
        float defaultElitismRate = 0.0;
        float defaultSelectionRate = 1.0;

        size_t maxGenerations = parameters.find("maxGenerations") != parameters.end() ? static_cast<size_t>(parameters.at("maxGenerations")) : defaultMaxGenerations;
        size_t optimizationFrequency = parameters.find("optimizationFrequency") != parameters.end() ? static_cast<size_t>(parameters.at("optimizationFrequency")) : defaultOptimizationFrequency;
        float elitismRate = parameters.find("elitismRate") != parameters.end() ? parameters.at("elitismRate") : defaultElitismRate;
        float selectionRate = parameters.find("selectionRate") != parameters.end() ? parameters.at("selectionRate") : defaultSelectionRate;

        std::shared_ptr<GeneticAlgorithm> ga = std::make_shared<GGAAC>(eval, 50, 15000, 0.08, 0.7);
        std::shared_ptr<LocalSearch> ls = std::make_shared<LocalSearch>(eval, 2, 15000, 0.0, 0.3);

        return std::make_unique<MemeticAlgorithm>(eval, ga, ls, maxGenerations, optimizationFrequency, elitismRate, selectionRate);
    }

    return nullptr;
}
