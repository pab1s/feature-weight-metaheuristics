#include "algorithms/SA.hpp"

#include <algorithm>
#include <cmath>

#include "utils/MathUtils.hpp"
#include "utils/RandomUtils.hpp"

SA::SA(std::shared_ptr<Evaluation> eval, size_t maxEvaluations, float finalTemperature, float acceptanceProbability, float worseningProbability)
     : eval(eval), maxEvaluations(maxEvaluations), finalTemperature(finalTemperature), acceptanceProbability(acceptanceProbability), worseningProbability(worseningProbability) {}

Solution SA::generateNeighbor(const Solution& solution) {
    Solution neighbor = solution;
    size_t n = solution.size();
    size_t index = RandomUtils::getRandomInt(0, n - 1);
    neighbor.weights.at(index) += RandomUtils::getRandomNormal(0.0f, std::sqrt(0.3f));
    neighbor.weights.at(index) = std::clamp(neighbor.weights.at(index), 0.0f, 1.0f);
    return neighbor;
}

EvaluatedSolution SA::run(const DataSet& dataset) {
    Solution initialSolution(dataset.getNumFeatures());
    return run(initialSolution, dataset);
}

EvaluatedSolution SA::run(const Solution& initialSolution, const DataSet& dataset) {
    size_t n = dataset.getNumFeatures();
    size_t maxNeighbors = 10 * n;
    size_t maxSuccess = 0.1 * maxNeighbors;
    const size_t M = maxEvaluations / maxNeighbors;

    Solution currentSolution = initialSolution;
    float currentFitness = eval->calculateFitnessLeaveOneOut(dataset, currentSolution);
    Solution bestSolution = currentSolution;
    float bestFitness = currentFitness;

    const float INITIAL_TEMPERATURE = (currentFitness * worseningProbability) / -std::log(acceptanceProbability);

    while (finalTemperature >= INITIAL_TEMPERATURE) {
        finalTemperature /= 10;
    }

    const float BETA = (INITIAL_TEMPERATURE - finalTemperature) / (M * INITIAL_TEMPERATURE * finalTemperature);
    float temperature = INITIAL_TEMPERATURE;
    size_t numEvaluations = 0;
    bool continueSearch = true;

    while (evaluations < maxEvaluations && continueSearch) {
        eval->insertFitnessRecord(bestFitness);
        size_t numSuccess = 0;
        size_t numNeighbors = 0;

        while (numSuccess < maxSuccess && numNeighbors < maxNeighbors && numEvaluations < maxEvaluations) {
            Solution neighbor = generateNeighbor(currentSolution);
            float neighborFitness = eval->calculateFitnessLeaveOneOut(dataset, neighbor);
            float delta = currentFitness - neighborFitness;
            numNeighbors++;
            numEvaluations++;

            if (delta < 0 || RandomUtils::getRandomFloat(0.0f, 1.0f) <= std::exp(-delta / temperature)) {
                currentSolution = neighbor;
                currentFitness = neighborFitness;
                numSuccess++;

                if (currentFitness > bestFitness) {
                    bestSolution = currentSolution;
                    bestFitness = currentFitness;
                }
            }
        }

        temperature = temperature / (1 + BETA * temperature);

        if (numSuccess == 0) {
            continueSearch = false;
        }
    }

    return EvaluatedSolution{bestSolution, bestFitness};
}
