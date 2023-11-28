#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=60:00
#SBATCH --mem=32G

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCa/Simple/Inf --debug-flags=ExecAll --debug-file=trace.out --debug-start=0 --debug-end=100000 gem5-config/run_micro.py Simple Inf microbenchmark/CCa/bench.X86