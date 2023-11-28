#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=60:00
#SBATCH --mem=32G

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCa/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 --debug-break=1000 gem5-config/run_micro.py Simple Inf microbenchmark/CCa/bench.X86