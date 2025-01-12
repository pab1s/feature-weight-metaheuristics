#include "data/DataLoader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>

std::string toLowerCase(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) { return std::tolower(c); });
    return lowerStr;
}

std::string normalizeClassLabel(const std::string& classLabel) {
    try {
        float value = std::stof(classLabel);
        if (std::floor(value) == value) {
            return std::to_string(static_cast<int>(value));
        }
    } catch (const std::invalid_argument& e) {
        // Do nothing
    }
    return classLabel;
}

namespace DataLoader {
    bool readARFF(const std::string& filename, DataSet& dataset) {
        std::ifstream file(filename);
        std::string line;
        bool isDataSection = false;

        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return false;
        }

        while (getline(file, line)) {
            std::string trimmedLine = toLowerCase(line);
            trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t"));
            trimmedLine.erase(trimmedLine.find_last_not_of(" \t") + 1);

            // Extract class labels
            if (trimmedLine.find("@attribute 'class'") != std::string::npos || 
                trimmedLine.find("@attribute class") != std::string::npos || 
                trimmedLine.find("@attribute 'target'") != std::string::npos) {
                size_t braceOpen = trimmedLine.find('{');
                size_t braceClose = trimmedLine.find('}', braceOpen);
                if (braceOpen != std::string::npos && braceClose != std::string::npos) {
                    std::string classesStr = trimmedLine.substr(braceOpen + 1, braceClose - braceOpen - 1);
                    std::stringstream ss(classesStr);
                    std::string classLabel;
                    while (getline(ss, classLabel, ',')) {
                        classLabel.erase(0, classLabel.find_first_not_of(" \t"));
                        classLabel.erase(classLabel.find_last_not_of(" \t") + 1);
                        dataset.addClassLabel(classLabel);
                    }
                }
                continue;
            }

            // Skip comments and empty lines
            if (trimmedLine.empty() || trimmedLine[0] == '%') {
                continue;
            }

            // Find data section
            if (trimmedLine.find("@data") != std::string::npos) {
                isDataSection = true;
                continue;
            }

            if (isDataSection) {
                std::stringstream ss(trimmedLine);
                std::vector<float> features;
                std::string value, rawClassLabel;
                // Parse features
                while (getline(ss, value, ',')) {
                    if (ss.peek() == EOF) {
                        // Last value is the class label
                        rawClassLabel = value;
                        // Normalize class label
                        std::string classLabel = normalizeClassLabel(rawClassLabel);
                        dataset.addItem(DataItem(features, classLabel));
                        break;
                    } else {
                        try {
                            features.push_back(std::stof(value));
                        } catch (const std::invalid_argument& e) {
                            std::cerr << "Invalid argument for stof: " << value << std::endl;
                        }
                    }
                }
            }
        }

        file.close();
        return true;
    }

    DataItem findMaxFeatureValues(const DataSet& dataset) {
        DataItem maxFeatureValue;
        if (dataset.size() == 0) {
            return maxFeatureValue;
        }

        maxFeatureValue.features = dataset.items[0].features;
        for (const auto& item : dataset.items) {
            for (size_t i = 0; i < item.features.size(); ++i) {
                if (item.features[i] > maxFeatureValue.features[i]) {
                    maxFeatureValue.features[i] = item.features[i];
                }
            }
        }

        return maxFeatureValue;
    }

    DataItem findMinFeatureValues(const DataSet& dataset) {
        DataItem minFeatureValue;
        if (dataset.size() == 0) {
            return minFeatureValue;
        }

        minFeatureValue.features = dataset.items[0].features;
        for (const auto& item : dataset.items) {
            for (size_t i = 0; i < item.features.size(); ++i) {
                if (item.features[i] < minFeatureValue.features[i]) {
                    minFeatureValue.features[i] = item.features[i];
                }
            }
        }

        return minFeatureValue;
    }

    void normalizeFeatures(DataSet& dataset, const DataItem& minFeatureValue, const DataItem& maxFeatureValue) {
        for (auto& item : dataset.items) {
            for (size_t i = 0; i < item.features.size(); ++i) {
                if (minFeatureValue.features[i] != maxFeatureValue.features[i]) {
                    item.features[i] = (item.features[i] - minFeatureValue.features[i]) / (maxFeatureValue.features[i] - minFeatureValue.features[i]);
                } else {
                    item.features[i] = 0.0;
                }
            }
        }
    }
}
