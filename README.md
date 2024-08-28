# Metaheuristics Weight Finding Project

## Overview

This project implements and evaluates various metaheuristic algorithms for weight finding in machine learning models, particularly for the Feature Weight Learning (FWL) problem. It includes a range of algorithms from simple local search methods to more complex population-based approaches, designed to optimize feature selection and weight assignment in classification tasks. The project uses 5-fold cross-validation (5CV) for analysis and comparison, focusing on weight generation for the 1-Nearest Neighbor (1NN) classification algorithm.

**NOTE**: The report is written in Spanish.

## Project Structure

```
.
├── bin/                  # Compiled binaries (contains metaheuristics executable)
├── data/                 # ARFF data files and FWL tables for 2023-24
├── docs/                 # Documentation
│   ├── html/             # Doxygen-generated HTML docs
│   └── latex/            # Doxygen-generated LaTeX docs
├── include/              # Header files
│   ├── algorithms/       # Algorithm-specific headers
│   └── utils/            # Utility headers
├── obj/                  # Compiled object files
├── outputs/              # Execution results, generated solutions, and training fitness logs
├── scripts/              # Utility scripts (including run.sh and generate_training_fitness_table.py)
├── src/                  # Source files
│   ├── algorithms/       # Algorithm implementations
│   └── utils/            # Utility implementations
├── tests/                # Unit tests and mocks
└── Makefile              # Project compilation instructions
```

## Datasets

The project evaluates the algorithms using three datasets:
- breast-cancer
- ecoli
- parkinsons

## Algorithms Implemented

### Local Search Methods
1. **Local Search**
2. **Best Local Search (BLS)**
3. **Simulated Annealing (SA)**
4. **Iterated Local Search (ILS)**
5. **ILS with SA (OILS-SA)**

### Population-based Methods
6. **Genetic Algorithm (GA)**
   - AGE-CA: Steady-state GA with arithmetic crossover
   - AGE-BLX: Steady-state GA with BLX-α crossover
   - AGG-CA: Generational GA with arithmetic crossover
   - AGG-BLX: Generational GA with BLX-α crossover
7. **Memetic Algorithm (MA)**
8. **MA with Restart**
9. **BMB (Baldwinian MA)**

### Other Methods
10. **1-Nearest Neighbor (1NN)**
11. **Relief**
12. **Random Search**

## Installation

1. Clone the repository:
   ```
   git clone https://github.com/pab1s/attribute-weight-metaheuristics.git
   cd mattribute-weight-metaheuristics
   ```
2. Compile the project:
   ```
   make
   ```

## Usage

Run the compiled binary with appropriate arguments:
```
./scripts/run.sh [options]
```

### Execution Parameters

The program accepts the following parameters:

- `--algorithm=ALGORITHM_NAME`: Specifies the algorithm to run (e.g., local-search, age-ca, memetic)
- `--dataset=DATASET_NAME`: Specifies the dataset to use (breast-cancer, ecoli, or parkinsons)
- `--seed=SEED_VALUE`: Sets the random seed
- `--log`: Enables logging of execution results

Additional algorithm-specific parameters can be specified. For example:

- Local Search:
  - `--maxEvaluations`: Maximum number of evaluations (default: 15000)
  - `--maxNeighbors`: Maximum number of neighbors to explore per iteration (default: 20)
  - `--variance`: Variance for the normal distribution in neighbor generation (default: 0.3)

- Genetic Algorithms:
  - `--maxEvaluations`: Maximum number of objective function evaluations (default: 15000)
  - `--populationSize`: Population size (default: 50)
  - `--crossoverRate`: Crossover probability (default: 1.0)
  - `--mutationRate`: Mutation probability (default: 0.08)

For specific usage instructions and parameters for each algorithm, please refer to the documentation in the `docs/` directory.

## Scripts

The `scripts/` directory contains useful scripts:
- `run.sh`: Main execution script
- `generate_training_fitness_table.py`: Generates performance graphs

To run the main experiment script:
```
./scripts/run.sh
```

## Documentation

This project uses Doxygen for documentation. To generate the documentation:
1. Ensure you have Doxygen installed.
2. Run:
   ```
   doxygen config.doxy
   ```
3. Access the generated documentation in `docs/html/index.html` or `docs/latex/refman.pdf`.

## Contact

Pablo Olivares Martínez - pablolivares1502@gmail.com
