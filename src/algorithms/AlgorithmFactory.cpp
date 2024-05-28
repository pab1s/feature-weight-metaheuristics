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
#include "algorithms/MA-restart.hpp"
#include "algorithms/BMB.hpp"
#include "algorithms/SA.hpp"
#include "algorithms/ILS.hpp"
#include "algorithms/ILS_SA.hpp"

std::unique_ptr<Algorithm> AlgorithmFactory::createAlgorithm(
    const std::string& name, 
    const std::map<std::string, float>& parameters,
    std::shared_ptr<Evaluation> eval
) {
    size_t maxEvaluations, populationSize;
    float crossoverRate, mutationRate;

    auto initializeParameters = [&parameters](size_t& maxEvaluations, size_t& populationSize, float& crossoverRate, float& mutationRate) {
        size_t defaultMaxEvaluations = 15000;
        size_t defaultPopulationSize = 50;
        float defaultCrossoverRate = 1.0;
        float defaultMutationRate = 0.08;

        maxEvaluations = parameters.find("maxGenerations") != parameters.end() ? static_cast<size_t>(parameters.at("maxGenerations")) : defaultMaxEvaluations;
        populationSize = parameters.find("populationSize") != parameters.end() ? static_cast<size_t>(parameters.at("populationSize")) : defaultPopulationSize;
        crossoverRate = parameters.find("crossoverRate") != parameters.end() ? parameters.at("crossoverRate") : defaultCrossoverRate;
        mutationRate = parameters.find("mutationRate") != parameters.end() ? parameters.at("mutationRate") : defaultMutationRate;
    };

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
    } else if (name == "age-blx") {
        initializeParameters(maxEvaluations, populationSize, crossoverRate, mutationRate);
        return std::make_unique<SGABLX>(eval, populationSize, maxEvaluations, mutationRate, crossoverRate);
    } else if (name == "age-ca") {
        initializeParameters(maxEvaluations, populationSize, crossoverRate, mutationRate);
        return std::make_unique<SGAAC>(eval, populationSize, maxEvaluations, mutationRate, crossoverRate);
    } else if (name == "agg-blx") {
        initializeParameters(maxEvaluations, populationSize, crossoverRate, mutationRate);
        return std::make_unique<GGABLX>(eval, populationSize, maxEvaluations, mutationRate, crossoverRate);
    } else if (name == "agg-ca") {
        initializeParameters(maxEvaluations, populationSize, crossoverRate, mutationRate);
        return std::make_unique<GGAAC>(eval, populationSize, maxEvaluations, mutationRate, crossoverRate);
    } else if (name == "memetic") {
        size_t defaultMaxEvaluations = 15000;
        size_t defaultOptimizationFrequency = 10;
        float defaultElitismRate = 0.0;
        float defaultSelectionRate = 1.0;

        size_t maxEvaluations = parameters.find("maxEvaluations") != parameters.end() ? static_cast<size_t>(parameters.at("maxEvaluations")) : defaultMaxEvaluations;
        size_t optimizationFrequency = parameters.find("optimizationFrequency") != parameters.end() ? static_cast<size_t>(parameters.at("optimizationFrequency")) : defaultOptimizationFrequency;
        float elitismRate = parameters.find("elitismRate") != parameters.end() ? parameters.at("elitismRate") : defaultElitismRate;
        float selectionRate = parameters.find("selectionRate") != parameters.end() ? parameters.at("selectionRate") : defaultSelectionRate;

        std::shared_ptr<GeneticAlgorithm> ga = std::make_shared<GGAAC>(eval, 50, 15000, 0.08, 0.7);
        std::shared_ptr<LocalSearch> ls = std::make_shared<LocalSearch>(eval, 2, 0, 0.0, 0.3);

        return std::make_unique<MemeticAlgorithm>(eval, ga, ls, maxEvaluations, optimizationFrequency, elitismRate, selectionRate);
    } else if (name == "memetic-restart") {
        size_t defaultMaxEvaluations = 15000;
        size_t defaultOptimizationFrequency = 10;
        float defaultElitismRate = 1.0;
        float defaultSelectionRate = 0.1;

        size_t maxEvaluations = parameters.find("maxEvaluations") != parameters.end() ? static_cast<size_t>(parameters.at("maxEvaluations")) : defaultMaxEvaluations;
        size_t optimizationFrequency = parameters.find("optimizationFrequency") != parameters.end() ? static_cast<size_t>(parameters.at("optimizationFrequency")) : defaultOptimizationFrequency;
        float elitismRate = parameters.find("elitismRate") != parameters.end() ? parameters.at("elitismRate") : defaultElitismRate;
        float selectionRate = parameters.find("selectionRate") != parameters.end() ? parameters.at("selectionRate") : defaultSelectionRate;

        std::shared_ptr<GeneticAlgorithm> ga = std::make_shared<GGAAC>(eval, 50, 15000, 0.1, 0.7);
        std::shared_ptr<LocalSearch> ls = std::make_shared<LocalSearch>(eval, 2, 0, 0.0, 0.3);

        return std::make_unique<MemeticRestart>(eval, ga, ls, maxEvaluations, optimizationFrequency, elitismRate, selectionRate);
    } else if (name == "bmb") {
        size_t defaultMaxEvaluations = 750;
        size_t defaultMaxIterations = 20;

        size_t maxEvaluations = parameters.find("maxEvaluations") != parameters.end() ? static_cast<size_t>(parameters.at("maxEvaluations")) : defaultMaxEvaluations;
        size_t maxIterations = parameters.find("maxIterations") != parameters.end() ? static_cast<size_t>(parameters.at("maxIterations")) : defaultMaxIterations;
        
        std::shared_ptr<LocalSearch> ls = std::make_shared<LocalSearch>(eval, 1, maxEvaluations, 0.0, 0.3);

        return std::make_unique<BMB>(eval, ls, maxIterations);
    } else if (name == "sa") {
        size_t defaultMaxEvaluations = 750;
        float defaultFinalTemperature = 0.001f;
        float defaultAcceptanceProbability = 0.3f;
        float defaultWorseningProbability = 0.1f;

        size_t maxEvaluations = parameters.find("maxEvaluations") != parameters.end() ? static_cast<size_t>(parameters.at("maxEvaluations")) : defaultMaxEvaluations;
        float finalTemperature = parameters.find("finalTemperature") != parameters.end() ? parameters.at("finalTemperature") : defaultFinalTemperature;
        float acceptanceProbability = parameters.find("acceptanceProbability") != parameters.end() ? parameters.at("acceptanceProbability") : defaultAcceptanceProbability;
        float worseningProbability = parameters.find("worseningProbability") != parameters.end() ? parameters.at("worseningProbability") : defaultWorseningProbability;

        return std::make_unique<SA>(eval, maxEvaluations, finalTemperature, acceptanceProbability, worseningProbability);
    } else if (name == "ils") {
        size_t defaultMaxEvaluations = 750;
        size_t defaultMaxIterations = 20;
        size_t defaultMutationLimit = 0.25f;

        size_t maxEvaluations = parameters.find("maxEvaluations") != parameters.end() ? static_cast<size_t>(parameters.at("maxEvaluations")) : defaultMaxEvaluations;
        size_t maxIterations = parameters.find("maxIterations") != parameters.end() ? static_cast<size_t>(parameters.at("maxIterations")) : defaultMaxIterations;
        float mutationLimit = parameters.find("mutationLimit") != parameters.end() ? parameters.at("mutationLimit") : defaultMutationLimit;

        std::shared_ptr<LocalSearch> ls = std::make_shared<LocalSearch>(eval, 1, maxEvaluations, 0.0, 0.3);

        return std::make_unique<ILS>(eval, ls, maxIterations, mutationLimit);
    } else if (name == "ils-sa") {
        size_t defaultMaxEvaluations = 750;
        size_t defaultMaxIterations = 20;
        size_t defaultMutationLimit = 0.25f;

        size_t maxEvaluations = parameters.find("maxEvaluations") != parameters.end() ? static_cast<size_t>(parameters.at("maxEvaluations")) : defaultMaxEvaluations;
        size_t maxIterations = parameters.find("maxIterations") != parameters.end() ? static_cast<size_t>(parameters.at("maxIterations")) : defaultMaxIterations;
        float mutationLimit = parameters.find("mutationLimit") != parameters.end() ? parameters.at("mutationLimit") : defaultMutationLimit;

        std::shared_ptr<SA> sa = std::make_shared<SA>(eval, maxEvaluations, 0.001f, 0.3f, 0.1f);

        return std::make_unique<ILSSA>(eval, sa, maxIterations, mutationLimit);
    }

    return nullptr;
}
