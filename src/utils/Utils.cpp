#include "utils/Utils.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#include "utils/json.hpp"

using json = nlohmann::json;

namespace Utils {
std::string generateFilename(const std::string& baseName,
                             const std::string& extension) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << baseName << "_"
       << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d_%H-%M-%S")
       << extension;
    return ss.str();
}

void logResult(size_t fold, float classificationRateTrain,
               float classificationRateTest, float reductionRate, float fitness,
               double duration, unsigned seed, const std::string& solutionID,
               const std::string& resultsFilename) {
    std::ofstream file(resultsFilename, std::ios::app);  // Append mode
    if (!file.is_open()) {
        std::cerr << "Error opening " << resultsFilename << std::endl;
        return;
    }

    file << fold << "," << classificationRateTrain << ","
         << classificationRateTest << "," << reductionRate << "," << fitness
         << "," << duration << "," << seed << "," << solutionID << "\n";

    file.close();
}

std::string saveSolutionToJSON(const std::vector<float>& solution) {
    std::string solutionID = std::to_string(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    json j;
    j["id"] = solutionID;
    j["weights"] = solution;

    std::string filename = "solution_" + solutionID + ".json";
    std::ofstream file("./outputs/solutions/" + filename);
    if (!file.is_open()) {
        std::cerr << "Error opening " << filename << std::endl;
        return "";
    }

    file << std::setw(4) << j << std::endl; // Indentation
    file.close();

    return solutionID;
}

void saveFitnessRecords(const std::string& filename, 
                        const std::vector<float>& fitnessRecords) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening " << filename << std::endl;
        return;
    }

    for (const auto& fitness : fitnessRecords) {
        file << fitness << "\n";
    }

    file.close();
}
}
