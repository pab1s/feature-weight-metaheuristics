#pragma once

#include "algorithms/KNNClassifier.hpp"

class OneNN : public KNNClassifier {
public:
    /** 
     * Constructor of the OneNN class with the euclidean distance.
     * 
     * @param originalTrainingData The training dataset.
     * @return An instance of the OneNN class.
    */
    OneNN(const DataSet& originalTrainingData);

    /**
     * Classifies a test item using the nearest neighbor algorithm.
     * 
     * @param testItem The test item to classify.
     * @param weights The weights of the features.
     * @return std::string The predicted label.
     */
    virtual std::string classify(const DataItem& testItem, const Solution& weights) const override;

    /**
     * Classifies a test item using the nearest neighbor algorithm, excluding the feature at the given index.
     * 
     * @param testItem The test item to classify.
     * @param excludeIndex The index of the feature to exclude.
     * @param solution The solution containing the weights of the features.
     * @return The predicted label.
     */
    virtual std::string classifyExcludingIndex(const DataItem& testItem, size_t excludeIndex, const Solution& solution) override;
};
