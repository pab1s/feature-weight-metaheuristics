#pragma once
#include <vector>
#include <string>

/**
 * @brief Represents a data item with features and a label.
 */
struct DataItem {
    std::vector<float> features; /**< The features of the data item. */
    std::string label; /**< The label of the data item. */

    /**
     * @brief Default constructor for DataItem to zero-initialize the features 
     * and set the label empty.
     */
    DataItem() : features(std::vector<float>()), label("") {}

    /**
     * @brief Constructor for DataItem for inference. Label is set empty.
     * @param features The features of the data item.
     */
    DataItem(std::vector<float> features)
        : features(features), label("") {}

    /**
     * @brief Constructor for DataItem.
     * @param features The features of the data item.
     * @param label The label of the data item.
     */
    DataItem(std::vector<float> features, std::string label)
        : features(features), label(label) {}
};
