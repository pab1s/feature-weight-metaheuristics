#pragma once
#include <vector>

/**
 * @brief The Solution class represents a solution for a problem.
 * 
 * This class stores a set of weights that can be used to solve a problem.
 * The weights are represented as a vector of floats.
 */
class Solution {
public:
    std::vector<float> weights; /**< The vector of weights of the solution **/

    /**
     * @brief Construct a new Solution object.
     * 
     * @param numberOfFeatures The number of features in the solution.
     */
    explicit Solution(size_t numberOfFeatures);

    /**
     * @brief Construct a new Solution object with a specified initial value.
     * 
     * @param numberOfFeatures The number of features in the solution.
     * @param value The initial value of the weights.
     */
    Solution(size_t numberOfFeatures, float value);

    /**
     * @brief Initialize the weights randomly.
     * 
     * This function initializes the weights with random values.
     */
    void initializeRandomly();

    /**
     * @brief Get the weight at the specified index.
     * 
     * @param index The index of the weight to get.
     * @return The weight at the specified index.
     */
    float getWeight(int index) const;

    /**
     * @brief Set the weight at the specified index.
     * 
     * @param index The index of the weight to set.
     * @param value The value to set the weight to.
     */
    void setWeight(int index, float value);

    /**
     * @brief Get the number of weights in the solution.
     * 
     * @return The number of weights in the solution.
     */
    size_t size() const;

    /**
     * @brief Overloaded assignment operator.
     * 
     * This function allows one Solution object to be assigned to another.
     * 
     * @param other The Solution object to assign.
     * @return The Solution object.
     */
    Solution& operator=(const Solution& other); 

    /**
     * @brief Overloaded output stream operator.
     * 
     * This function allows the Solution object to be printed to an output stream.
     * 
     * @param os The output stream.
     * @param solution The Solution object to print.
     * @return The output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Solution& solution);
};
