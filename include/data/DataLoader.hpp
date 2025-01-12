#pragma once

#include "data/DataSet.hpp"
#include <string.h>

/**
 * @brief Namespace for data loading operations.
 */
namespace DataLoader {
    /**
     * @brief Reads an ARFF file and populates a DataSet object.
     * 
     * @param filename The path to the ARFF file.
     * @param dataset The DataSet object to populate.
     * @return True if the file was successfully read and the DataSet was populated, false otherwise.
     */
    bool readARFF(const std::string& filename, DataSet& dataset);

    /**
     * @brief Finds the minimum feature values in a dataset.
     * 
     * @param dataset The dataset to find the minimum feature values.
     * @return The minimum feature values.
     */
    DataItem findMinFeatureValues(const DataSet& dataset);

    /**
     * @brief Finds the maximum feature values in a dataset.
     * 
     * @param dataset The dataset to find the maximum feature values.
     * @return The maximum feature values.
     */
    DataItem findMaxFeatureValues(const DataSet& dataset);

    /**
     * @brief Normalizes the features of a dataset.
     * 
     * @param dataset The dataset to normalize.
     * @param minFeatureValues The minimum feature values.
     * @param maxFeatureValues The maximum feature values.
     */
    void normalizeFeatures(DataSet& dataset, const DataItem& minFeatureValues, const DataItem& maxFeatureValues);
}
