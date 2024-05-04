# This script is used to run a program with different datasets and algorithms.
# It checks if the provided dataset and algorithm are valid, and if the program executable exists.
# If a dataset and algorithm are provided as command line arguments, it runs the program with those arguments.
# If no arguments are provided, it runs the program with all possible combinations of datasets and algorithms.
# Author: Pablo Olivares Martínez
# Date: 2024/03/25

#!/bin/bash

# Define the valid datasets and algorithms
datasets="ecoli parkinsons breast-cancer"
algorithms="1nn relief local-search best-local-search age-blx"

# Function to verify if the value is a valid dataset
is_valid_dataset() {
    [[ " $datasets " =~ " $1 " ]] && return 0 || return 1
}

# Function to verify if the value is a valid algorithm
is_valid_algorithm() {
    [[ " $algorithms " =~ " $1 " ]] && return 0 || return 1
}

# Process the command line arguments
command="./bin/metaheuristics"
log=""
algorithm_provided=false
dataset_provided=false

for arg in "$@"; do
    case $arg in
        --algorithm=*)
            algorithm="${arg#*=}"
            if is_valid_algorithm "$algorithm"; then
                command+=" --algorithm=$algorithm"
                algorithm_provided=true
            else
                echo "Error: Invalid algorithm. Valid algorithms: $algorithms."
                exit 1
            fi
            ;;
        --dataset=*)
            dataset="${arg#*=}"
            if is_valid_dataset "$dataset"; then
                command+=" --dataset=$dataset"
                dataset_provided=true
            else
                echo "Error: Invalid dataset. Valid datasets: $datasets."
                exit 1
            fi
            ;;
        --log)
            log="--log"
            ;;
        *)
            # Add the argument to the command
            command+=" $arg"
            ;;
    esac
done

# Verify if the program executable exists
if [ ! -x "./bin/metaheuristics" ]; then
    echo "Error: Program executable not found."
    exit 1
fi

# Execute the program with the provided arguments
if [ "$algorithm_provided" = false ] && [ "$dataset_provided" = false ]; then
    read -p "El programa se ejecutará para todos los algoritmos y datasets disponibles. ¿Desea continuar? Responde y para sí o n para no: " response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        for alg in $algorithms; do
            for dset in $datasets; do
                eval $command --algorithm="$alg" --dataset="$dset" $log
            done
        done
    else
        exit 0
    fi
elif [ "$algorithm_provided" = true ] && [ "$dataset_provided" = false ]; then
    echo "El programa se ejecutará para el algoritmo $algorithm con todos los datasets disponibles."
    for dset in $datasets; do
        eval $command --dataset="$dset" $log
    done
elif [ "$algorithm_provided" = false ] && [ "$dataset_provided" = true ]; then
    echo "El programa se ejecutará para el dataset $dataset con todos los algoritmos disponibles."
    for alg in $algorithms; do
        eval $command --algorithm="$alg" $log
    done
else
    eval $command $log
fi
