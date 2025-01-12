#include "algorithms/CMAES.hpp"
#include "utils/RandomUtils.hpp"
#include <cmath>
#include <algorithm>

CMAES::CMAES(std::shared_ptr<Evaluation> eval, size_t maxEvaluations, size_t lambda)
    : eval(eval), maxEvaluations(maxEvaluations), lambda(lambda) {}

void CMAES::initializeParameters(size_t dimension) {
    if (lambda == 0) {
        lambda = 4 + static_cast<size_t>(std::floor(3 * std::log(dimension)));
    }
    mu = lambda / 2;

    // Initialize weights
    weights.resize(mu);
    for (size_t i = 0; i < mu; i++) {
        weights[i] = std::log(lambda / 2.0 + 0.5) - std::log(i + 1.0);
    }
    
    // Normalize weights
    float sum = std::accumulate(weights.begin(), weights.end(), 0.0f);
    for (auto& w : weights) {
        w /= sum;
    }

    // Compute effective mu
    mueff = 0.0f;
    for (float w : weights) {
        mueff += w * w;
    }
    mueff = 1.0f / mueff;

    // Strategy parameter setting
    cc = (4 + mueff/dimension) / (dimension + 4 + 2*mueff/dimension);
    cs = (mueff + 2) / (dimension + mueff + 5);
    c1 = 2 / ((dimension + 1.3)*(dimension + 1.3) + mueff);
    cmu = std::min(1 - c1, 2 * (mueff - 2 + 1/mueff) / ((dimension + 2)*(dimension + 2) + mueff));
    damps = 1 + 2*std::max(0.0f, std::sqrt((mueff - 1)/(dimension + 1)) - 1) + cs;
    chiN = std::sqrt(dimension) * (1 - 1/(4*dimension) + 1/(21*dimension*dimension));
}

std::vector<Solution> CMAES::generatePopulation(const Eigen::VectorXf& mean,
                                              float sigma,
                                              const Eigen::MatrixXf& C,
                                              size_t dimension) {
    Eigen::LLT<Eigen::MatrixXf> cholDecomp(C);
    Eigen::MatrixXf L = cholDecomp.matrixL();
    
    std::vector<Solution> population(lambda, Solution(dimension));
    
    for (size_t i = 0; i < lambda; i++) {
        Eigen::VectorXf z(dimension);
        for (size_t j = 0; j < dimension; j++) {
            z(j) = RandomUtils::getRandomNormal(0.0f, 1.0f);
        }
        
        Eigen::VectorXf x = mean + sigma * (L * z);
        for (size_t j = 0; j < dimension; j++) {
            population[i].weights[j] = std::clamp(x(j), 0.0f, 1.0f);
        }
    }
    
    return population;
}

std::vector<size_t> CMAES::getSortedIndices(const std::vector<float>& fitness) {
    std::vector<size_t> indices(fitness.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
              [&](size_t a, size_t b) { return fitness[a] > fitness[b]; });
    return indices;
}

EvaluatedSolution CMAES::run(const DataSet& dataset) {
    size_t dimension = dataset.getNumFeatures();
    initializeParameters(dimension);
    
    // Initialize strategy parameters
    Eigen::VectorXf mean = Eigen::VectorXf::Constant(dimension, 0.5);
    Eigen::MatrixXf C = Eigen::MatrixXf::Identity(dimension, dimension);
    Eigen::VectorXf pc = Eigen::VectorXf::Zero(dimension);
    Eigen::VectorXf ps = Eigen::VectorXf::Zero(dimension);
    float sigma = 0.3;
    
    Solution bestSolution(dimension);
    float bestFitness = std::numeric_limits<float>::lowest();
    
    while (evaluations < maxEvaluations) {
        // Generate and evaluate population
        auto population = generatePopulation(mean, sigma, C, dimension);
        std::vector<float> fitness(lambda);
        
        for (size_t i = 0; i < lambda; i++) {
            fitness[i] = eval->calculateFitnessLeaveOneOut(dataset, population[i]);
            evaluations++;
            
            if (fitness[i] > bestFitness) {
                bestFitness = fitness[i];
                bestSolution = population[i];
            }
        }
        
        eval->insertFitnessRecord(bestFitness);
        
        // Sort by fitness and compute weighted mean
        auto indices = getSortedIndices(fitness);
        Eigen::VectorXf oldMean = mean;
        mean.setZero();
        
        for (size_t i = 0; i < mu; i++) {
            for (size_t j = 0; j < dimension; j++) {
                mean(j) += weights[i] * population[indices[i]].weights[j];
            }
        }
        
        // Update evolution paths
        Eigen::VectorXf y = (mean - oldMean) / sigma;
        ps = (1 - cs) * ps + std::sqrt(cs * (2 - cs) * mueff) * C.llt().matrixL().solve(y);
        float hsig = (ps.norm() / std::sqrt(1 - std::pow(1 - cs, 2.0f * evaluations / lambda)) 
                     / chiN < 1.4f + 2.0f / (dimension + 1)) ? 1.0f : 0.0f;
        pc = (1 - cc) * pc + hsig * std::sqrt(cc * (2 - cc) * mueff) * y;
        
        // Adapt covariance matrix
        C = (1 - c1 - cmu) * C 
            + c1 * (pc * pc.transpose() 
            + (1 - hsig) * cc * (2 - cc) * C);
            
        for (size_t i = 0; i < mu; i++) {
            Eigen::VectorXf dv(dimension);
            for (size_t j = 0; j < dimension; j++) {
                dv(j) = (population[indices[i]].weights[j] - oldMean(j)) / sigma;
            }
            C += cmu * weights[i] * dv * dv.transpose();
        }
        
        // Update sigma
        sigma *= std::exp((cs/damps) * (ps.norm()/chiN - 1));
    }
    
    return EvaluatedSolution{bestSolution, bestFitness};
} 