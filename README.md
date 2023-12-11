# README

The link to our repo can be found [here](https://github.com/DerpyWasHere/CMPT-450-Project).

## Precursors

Run the following in the repository home directory

    export PROJECT_DIR=$PWD

## Slurm

All scripts are ran using Slurm with the following parameters
- cpus-per-task=8
- time=60:00
- mem=32G

## Compiling 

In cases where the benchmarks need to be recompiled, run the following in the repository's home directory

    cd microbenchmark
    make all

## Building

To build gem5, run the following in the repository's home directory

    sbatch ./build.sh

This may take up to 30 minutes.

## Benchmarks

To generate benchmark results, run the following from the repository's home directory, taking care to run two lines at a time until completion to satisfy job quotas

    # Simple CPU
    sbatch SimpleInfBenchmark.sh
    sbatch SimpleSlowBenchmark.sh

    # DefaultO3
    sbatch DefaultO3InfBenchmark.sh
    sbatch DefaultO3SlowBenchmark.sh

    # O3_W2K
    sbatch O3_W2KInfBenchmark.sh
    sbatch O3_W2KSlowBenchmark.sh

    # O3_W256
    sbatch O3_W256InfBenchmark.sh
    sbatch O3_W256SlowBenchmark.sh

## Plotting

Run every python script under the `/plots` directory. Alternatively, run the `plot.sh` script while in the `/plots` directory

    cd plots
    ./plot.sh