#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <chrono>
#include "algorithms/AlgorithmFactory.hpp"
#include "data/DataLoader.hpp"
#include "data/DataSet.hpp"
#include "utils/Utils.hpp"
#include "utils/RandomUtils.hpp"
#include "Evaluation.hpp"

void processFold(std::ofstream& csvFile, const std::string& datasetName,
                 size_t fold, const DataSet& train, const DataSet& test,
                 std::shared_ptr<Evaluation> eval, std::unique_ptr<Algorithm> algorithm, 
                 const std::string& algorithmName, unsigned seed,
                 const std::string& resultsFilename, bool logResults);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " --algorithm=ALGORITHM_NAME --dataset=DATASET_NAME [--param1=value1 ...]" << std::endl;
        return 1;
    }

    // Parse arguments
    std::map<std::string, std::string> argsMap;
    bool logResults = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        auto pos = arg.find('=');

        if (arg == "--log") {
            logResults = true;
            continue;
        }

        if (pos != std::string::npos) {
            std::string key = arg.substr(2, pos - 2); // Remove '--' prefix
            std::string value = arg.substr(pos + 1);
            argsMap[key] = value;
        }
    }

    if (argsMap.find("algorithm") == argsMap.end() || argsMap.find("dataset") == argsMap.end()) {
        std::cerr << "Missing required arguments --algorithm or --dataset." << std::endl;
        return 1;
    }

    std::string algorithmName = argsMap["algorithm"];
    std::string datasetName = argsMap["dataset"];

    // Convert parameters to float
    std::map<std::string, float> parameters;
    for (const auto& param : argsMap) {
        if (param.first != "algorithm" && param.first != "dataset") {
            try {
                parameters[param.first] = std::stof(param.second);
            } catch (...) {
                std::cerr << "Error converting parameter " << param.first << " to float." << std::endl;
                return 1;
            }
        }
    }

    float defaultAlpha = 0.75;
    if (parameters.find("alpha") == parameters.end()) {
        parameters["alpha"] = defaultAlpha;
    }

    float defaultRedThreshold = 0.1f;
    if (parameters.find("reductionThreshold") == parameters.end()) {
        parameters["reductionThreshold"] = defaultRedThreshold;
    }

    unsigned randomSeed = RandomUtils::getRandomInt(0, 1000);
    if (parameters.find("seed") == parameters.end()) {
        parameters["seed"] = randomSeed;
    }  

    std::cout << "\n--------------------------------------------------" << std::endl;
    std::cout << "ALGORITHM: " << algorithmName << std::endl;
    std::cout << "DATASET: " << datasetName << std::endl;
    std::cout << "HYPERPARAMETERS:" << std::endl;
    for (const auto& param : parameters) {
        std::cout << param.first << ": " << param.second << std::endl;
    }

    RandomUtils::seedRandom(parameters["seed"]);

    std::string resultsFilename;
    std::ofstream csvFile;

    if (logResults) {
        resultsFilename = "./outputs/results/" + Utils::generateFilename(datasetName + "_" + algorithmName, ".csv");
        csvFile.open(resultsFilename);
        csvFile << "fold,train_class_rate,test_class_rate,red_rate,fitness,time,seed,solutionID\n";
        csvFile.close();
    }
    
    std::vector<std::string> dataFiles = {"1.arff", "2.arff", "3.arff", "4.arff", "5.arff"};

    for (size_t testIndex = 0; testIndex < dataFiles.size(); ++testIndex) {
        DataSet train, test;
        DataLoader::readARFF("./data/" + datasetName + "_" + dataFiles[testIndex], test);
        for (size_t i = 0; i < dataFiles.size(); ++i) {
            if (i != testIndex) {
                DataLoader::readARFF("./data/" + datasetName + "_" + dataFiles[i], train);
            }
        }

        DataItem max = DataLoader::findMaxFeatureValues(train);
        DataItem min = DataLoader::findMinFeatureValues(train);
        DataItem maxTest = DataLoader::findMaxFeatureValues(test);
        DataItem minTest = DataLoader::findMinFeatureValues(test);

        for (size_t i = 0; i < max.features.size(); ++i) {
            max.features.at(i) = std::max(max.features.at(i), maxTest.features.at(i));
            min.features.at(i) = std::min(min.features.at(i), minTest.features.at(i));
        }

        DataLoader::normalizeFeatures(train, min, max);
        DataLoader::normalizeFeatures(test, min, max);

        std::shared_ptr<Evaluation> eval = std::make_shared<Evaluation>(train, parameters["alpha"], parameters["reductionThreshold"]);
        auto algorithm = AlgorithmFactory::createAlgorithm(algorithmName, parameters, eval);
        if (!algorithm) {
            std::cerr << "Unknown algorithm: " << algorithmName << std::endl;
            return 1;
        }

        processFold(csvFile, datasetName, testIndex+1, train, test, eval, std::move(algorithm), 
                    algorithmName, parameters["seed"], resultsFilename, logResults);
    }

    std::cout << "--------------------------------------------------" << std::endl;

    return 0;
}

void processFold(std::ofstream& csvFile, const std::string& datasetName,
                 size_t fold, const DataSet& train, const DataSet& test,
                 std::shared_ptr<Evaluation> eval, std::unique_ptr<Algorithm> algorithm, 
                 const std::string& algorithmName, unsigned seed,
                 const std::string& resultsFilename, bool logResults) {

    std::cout << "\nProcessing fold: " << fold << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    EvaluatedSolution solution = algorithm->run(train);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    float classificationRateTrain = eval->leaveOneOutCrossValidation(train, solution.solution);
    float classificationRateTest = eval->calculateClassificationRate(test, solution.solution);
    float reductionRate = eval->calculateReductionRate(solution.solution);
    float fitness = eval->calculateFitness(classificationRateTest, reductionRate);

    std::cout << "Solution: " << solution.solution << std::endl;
    std::cout << "Training fitness value: " << solution.fitness << std::endl;
    std::cout << "Test fitness value: " << fitness << std::endl;
    std::cout << "Execution time: " << duration.count() << "s" << std::endl;

    if (logResults) {
        std::string solutionID = Utils::saveSolutionToJSON(solution.solution.weights);
        std::string fitnessFilename = "./outputs/training_fitness/" + solutionID + "_" + datasetName + "_fold_" + std::to_string(fold) + ".csv";
        Utils::saveFitnessRecords(fitnessFilename, eval->getFitnessRecords());
        Utils::logResult(fold, classificationRateTrain, classificationRateTest, 
                         reductionRate, fitness, duration.count(), seed, solutionID, resultsFilename);        
    }
}
