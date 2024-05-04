#pragma once
#include <string>
#include <vector>

#include "utils/json.hpp"

namespace Utils {
/**
 * @brief Generates a unique ID based on the current time in microseconds.
 *
 * @return std::string The generated unique ID.
 */
std::string generateFilename(const std::string& baseName,
                             const std::string& extension);

/**
 * @brief Saves a solution to a file.
 *
 * @param solutionID The ID of the solution.
 * @param solution The solution to save.
 */
void logResult(size_t fold, float classificationRateTrain,
               float classificationRateTest, float reductionRate, float fitness,
               double duration, unsigned seed, const std::string& solutionID,
               const std::string& resultsFilename);

/**
 * @brief Saves a solution to a JSON file.
 *
 * @param solution The solution to save.
 * @return std::string The solution identifier.
 */
std::string saveSolutionToJSON(const std::vector<float>& solution);

/**
 * @brief Saves the fitness records to a file.
 *
 * @param filename The name of the file to save the fitness records.
 * @param fitnessRecords The fitness records to save.
 */
void saveFitnessRecords(const std::string& filename, const std::vector<float>& fitnessRecords);
}
