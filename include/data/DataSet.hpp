#pragma once
#include "data/DataItem.hpp"
#include <set>
#include <iostream>

/**
 * @class DataSet
 * @brief Represents a collection of data items.
 */
class DataSet {
public:
    std::vector<DataItem> items; /**< The vector of data items in the dataset. */
    std::set<std::string> uniqueClasses; /**< The set of unique class labels in the dataset. */
    DataItem minFeatureValue; /**< The minimum feature value in the dataset. */
    DataItem maxFeatureValue; /**< The maximum feature value in the dataset. */

    /**
     * @brief Empty DataSet constructor.
     */
    DataSet();

    /**
     * @brief Returns the number of features in the dataset.
     * @return The number of features.
     */
    int getNumFeatures() const;

    /**
     * @brief Sets the maximum feature value in the dataset.
     * @param maxFeatureValue The maximum feature value.
     */
    void setMaxFeatureValues(DataItem maxFeatureValue);

    /**
     * @brief Sets the minimum feature value in the dataset.
     * @param minFeatureValue The minimum feature value.
     */
    void setMinFeatureValues(DataItem minFeatureValue);

    /**
     * @brief Gets the maximum feature values in the dataset.
     * @return The maximum feature values.  
     */
    DataItem getMaxFeatureValues();

    /**
     * @brief Gets the minimum feature values in the dataset.
     * @return The minimum feature values.
     */
    DataItem getMinFeatureValues();

    /**
     * @brief Adds a data item to the dataset.
     * @param item The data item to be added.
     */
    void addItem(const DataItem& item);

    /**
     * @brief Adds a dataset to the current dataset.
     * @param dataset The dataset to be added.
     */
    void addDataSet(const DataSet& dataset);

    /**
     * @brief Returns the number of data items in the dataset.
     * @return The number of data items.
     */
    size_t size() const;

    /**
     * @brief Adds a class label to the dataset.
     * @param classLabel The class label to be added.
     */
    void addClassLabel(const std::string& classLabel);

    /**
     * @brief Checks if a class label exists in the dataset.
     * @param classLabel The class label to be checked.
     * @return True if the class label exists, false otherwise.
     */
    bool classLabelExists(const std::string& classLabel) const;

    /**
     * @brief Clears the dataset.
     */
    void clear();

    /**
     * @brief Accesses the data item at the specified index.
     * @param index The index of the data item.
     * @return A reference to the data item.
     */
    DataItem& operator[](int index);

    /**
     * @brief Accesses the data item at the specified index (const version).
     * @param index The index of the data item.
     * @return A const reference to the data item.
     */
    const DataItem& operator[](int index) const;

    /**
     * @brief Operator overload for the output stream.
     * @param os The output stream.
     * @param dataset The dataset to be output.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const DataSet& dataset);
};

/**
 * @brief Outputs the dataset to the output stream.
 * @param os The output stream.
 * @param dataset The dataset to be output.
 * @return The output stream.
 */
inline std::ostream& operator<<(std::ostream& os, const DataSet& dataset) {
    for (const auto& item : dataset.items) {
        // Explicit stream for DataItem
        os << "DataItem: " << "{[";
        for (size_t i = 0; i < item.features.size(); ++i) {
            os << item.features[i];
            if (i < item.features.size() - 1) {
                os << ", ";
            }
        }
        os << "], " << item.label << "}" << std::endl;
    }
    return os;
}

/**
 * @brief Creates a test dataset with predefined data items.
 * @return The test dataset.
 */
inline DataSet createTestDataSet() {
    DataSet dataset;
    dataset.addItem(DataItem({0.5, 0.2, 0.1}, "a"));
    dataset.addItem(DataItem({0.9, 0.8, 0.2}, "b"));
    
    return dataset;
}
