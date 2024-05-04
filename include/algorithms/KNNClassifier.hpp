#pragma once

#include "data/DataSet.hpp"
#include "Solution.hpp"

class KNNClassifier {
protected:
    DataSet trainingData;
    size_t k;

public:
    /** 
     * Constructor of the KNNClassifier class.
     * 
     * @param originalTrainingData The training dataset.
     * @param k The number of neighbors to consider (default: 1).
     * @return An instance of the KNNClassifier class.
    */
    KNNClassifier(const DataSet& originalTrainingData, size_t k = 1);

    /**
     * Returns the number of neighbors to consider.
     * 
     * @return The number of neighbors to consider.
     */
    size_t getK() const;

    /**
     * Classifies a test item using the k-nearest neighbors algorithm.
     * 
     * @param testItem The test item to classify.
     * @param weights The weights of the features.
     * @return std::string The predicted label.
     */
    virtual std::string classify(const DataItem& testItem, const Solution& weights) const;

    /**
     * Classifies a test item using the k-nearest neighbors algorithm, excluding the feature at the given index.
     * 
     * @param testItem The test item to classify.
     * @param excludeIndex The index of the feature to exclude.
     * @param solution The solution containing the weights of the features.
     * @return The predicted label.
     */
    virtual std::string classifyExcludingIndex(const DataItem& testItem, size_t excludeIndex, const Solution& solution);
};
