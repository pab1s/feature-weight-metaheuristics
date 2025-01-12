#include "data/DataSet.hpp"
#include <iostream>

DataSet::DataSet() {
    items = std::vector<DataItem>();
    uniqueClasses = std::set<std::string>();
    minFeatureValue = DataItem();
    maxFeatureValue = DataItem();
}

int DataSet::getNumFeatures() const {
    if (items.empty()) {
        return 0;
    }
    return items[0].features.size();
}

void DataSet::setMaxFeatureValues(DataItem maxFeatureValue) {
    this->maxFeatureValue = maxFeatureValue;
}

void DataSet::setMinFeatureValues(DataItem minFeatureValue) {
    this->minFeatureValue = minFeatureValue;
}

DataItem DataSet::getMaxFeatureValues() {
    return maxFeatureValue;
}

DataItem DataSet::getMinFeatureValues() {
    return minFeatureValue;
}

void DataSet::addItem(const DataItem& item) {
    if (uniqueClasses.find(item.label) != uniqueClasses.end()) {
        items.push_back(item);
    } else {
        std::cerr << "Error: Class label '" << item.label << "' does not exist in the dataset's known classes." << std::endl;
    }
}

void DataSet::addDataSet(const DataSet& dataset) {
    for (const auto& item : dataset.items) {
        addItem(item);
    }
}

size_t DataSet::size() const {
    return items.size();
}

void DataSet::addClassLabel(const std::string& classLabel) {
    uniqueClasses.insert(classLabel);
}

bool DataSet::classLabelExists(const std::string& classLabel) const {
    return uniqueClasses.find(classLabel) != uniqueClasses.end();
}

void DataSet::clear() {
    items.clear();
    uniqueClasses.clear();
}

DataItem& DataSet::operator[](int index) {
    return items[index];
}

const DataItem& DataSet::operator[](int index) const {
    return items[index];
}
