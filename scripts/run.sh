#!/bin/bash

# Define the valid datasets and algorithms
datasets="ecoli parkinsons breast-cancer"
algorithms="1nn relief local-search best-local-search age-ca age-blx agg-ca agg-blx memetic memetic-restart bmb sa ils ils-sa"

# Function to verify if the value is a valid dataset
is_valid_dataset() {
    [[ " $datasets " =~ " $1 " ]] && return 0 || return 1
}

# Function to verify if the value is a valid algorithm
is_valid_algorithm() {
    [[ " $algorithms " =~ " $1 " ]] && return 0 || return 1
}

# Add specific algorithm parameters based on the type if not provided by the user
add_algorithm_parameters() {
    local alg=$1
    local params=$2

    # Defaults if not provided
    local crossoverRate=""
    local mutationRate=""

    if [[ $alg == agg-* ]]; then
        [[ $params != *"crossoverRate"* ]] && crossoverRate="--crossoverRate=0.68"
        [[ $params != *"mutationRate"* ]] && mutationRate="--mutationRate=0.08"
    elif [[ $alg == age-* ]]; then
        [[ $params != *"crossoverRate"* ]] && crossoverRate="--crossoverRate=1.0"
        [[ $params != *"mutationRate"* ]] && mutationRate="--mutationRate=0.08"
    fi

    echo "$crossoverRate $mutationRate"
}

# Special handling for memetic algorithm with different configurations
execute_memetic() {
    local algorithm=$1
    local dataset=$2
    local log_option=$3

    # Define memetic configurations
    local configurations=(
        "--elitismRate=0 --selectionRate=1"
        "--elitismRate=0 --selectionRate=0.1"
        "--elitismRate=1 --selectionRate=0.1"
    )

    for config in "${configurations[@]}"; do
        eval $command --algorithm=$algorithm $config --dataset=$dataset $log_option
    done
}

# Process the command line arguments
command="./bin/metaheuristics"
log=""
algorithm_provided=false
dataset_provided=false
extra_params=""
algorithm=""
dataset=""

for arg in "$@"; do
    case $arg in
        --algorithm=*)
            algorithm="${arg#*=}"
            if is_valid_algorithm "$algorithm"; then
                algorithm_provided=true
            else
                echo "Error: Invalid algorithm. Valid algorithms: $algorithms."
                exit 1
            fi
            ;;
        --dataset=*)
            dataset="${arg#*=}"
            if is_valid_dataset "$dataset"; then
                dataset_provided=true
            else
                echo "Error: Invalid dataset. Valid datasets: $datasets."
                exit 1
            fi
            ;;
        --log)
            log="--log"
            ;;
        --crossoverRate=*|--mutationRate=*)
            extra_params+=" $arg"
            ;;
        *)
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
if [ "$algorithm_provided" = true ]; then
    if [ "$dataset_provided" = true ]; then
        if [[ "$algorithm" == "memetic" ]]; then
            execute_memetic "$algorithm" "$dataset" "$log"
        else
            algorithm_parameters=$(add_algorithm_parameters "$algorithm" "$extra_params")
            eval $command --algorithm=$algorithm $algorithm_parameters --dataset=$dataset $log
        fi
    else
        for dset in $datasets; do
            if [[ "$algorithm" == "memetic" ]]; then
                execute_memetic "$algorithm" "$dset" "$log"
            else
                algorithm_parameters=$(add_algorithm_parameters "$algorithm" "$extra_params")
                eval $command --algorithm=$algorithm $algorithm_parameters --dataset="$dset" $log
            fi
        done
    fi
else
    for alg in $algorithms; do
        for dset in $datasets; do
            if [[ "$alg" == "memetic" ]]; then
                execute_memetic "$alg" "$dset" "$log"
            else
                algorithm_parameters=$(add_algorithm_parameters "$alg" "$extra_params")
                eval $command --algorithm="$alg" $algorithm_parameters --dataset="$dset" $log
            fi
        done
    done
fi