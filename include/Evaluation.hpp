#pragma once

#include "data/DataSet.hpp"
#include "Solution.hpp"
#include "algorithms/1NN.hpp"
#include "data/EvaluatedSolution.hpp"
#include <vector>

class Evaluation {
private:
    OneNN nn;
    float alpha;
    float reductionThreshold;
    std::vector<float> fitnessRecords;

public:
    /**
     * @brief Constructor of the Evaluation class.
     * 
     * @param trainingData The training dataset.
     * @param alpha The alpha value.
     * @param reductionThreshold The reduction threshold.
     */
    Evaluation(const DataSet& trainingData, float alpha, float reductionThreshold);

    /**
     * @brief Returns the fitness records.
     * 
     * @return The fitness records.
     */
    std::vector<float> getFitnessRecords() const;

    /**
     * @brief Inserts a fitness record.
     * 
     * @param fitness The fitness value to insert.
     */
    void insertFitnessRecord(float fitness);

    /**
     * @brief Reduces the solution by setting weights below a threshold to zero.
     * 
     * @param solution The solution to reduce.
     * @return The reduced solution.
     */
    Solution reduceSolution(const Solution& solution);

    /**
     * @brief Evaluates the fitness of a solution using leave-one-out cross-validation.
     * 
     * @param dataset The dataset on which the solution will be evaluated.
     * @param solution The solution to be evaluated.
     * @return The fitness of the solution.
     */
    float leaveOneOutCrossValidation(const DataSet& dataset, const Solution& solution);

    /**
     * @brief Calculates the classification rate for a given dataset and solution.
     *
     * @param dataset The dataset to evaluate.
     * @param solution The solution to evaluate.
     * @return The classification rate.
     */
    float calculateClassificationRate(const DataSet& dataset, const Solution& solution);

    /**
     * @brief Calculates the reduction rate for a given solution.
     *
     * @param solution The solution to evaluate.
     * @return The reduction rate.
     */
    float calculateReductionRate(const Solution& solution);

    /**
     * @brief Calculates the fitness value for a given solution.
     *
     * @param dataset The dataset to evaluate.
     * @param solution The solution to evaluate.
     * @return The fitness value.
     */
    float calculateFitness(const DataSet& dataset, const Solution& solution);

    /**
     * @brief Calculates the fitness value for a given classification rate, reduction rate, and alpha value.
     *
     * @param classificationRate The classification rate.
     * @param reductionRate The reduction rate.
     * @return The fitness value.
     */
    float calculateFitness(float classificationRate, float reductionRate);

    /**
     * @brief Calculates the fitness value for a given solution using leave-one-out cross-validation.
     *
     * @param dataset The dataset to evaluate.
     * @param solution The solution to evaluate.
     * @return The fitness value.
     */
    float calculateFitnessLeaveOneOut(const DataSet& dataset, const Solution& solution);

    /**
     * @brief Evaluates a solution using the classification rate, reduction rate, and alpha value.
     *
     * @param solution The solution to evaluate.
     * @param dataset The dataset to evaluate.
     * @return The evaluated solution.
     */
    EvaluatedSolution evaluate(const Solution& solution, const DataSet& dataset);

    /**
     * @brief Evaluates the population using the classification rate, reduction rate, and alpha value.
     *
     * @param population The population to evaluate.
     * @param dataset The dataset to evaluate.
     * @return A vector of fitness values.
     */
    std::vector<float> evaluatePopulation(const std::vector<Solution>& population, const DataSet& dataset);

    /**
     * @brief Clears the fitness records.
     */
    void clearFitnessRecords();
};
