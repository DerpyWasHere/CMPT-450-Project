#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=60:00
#SBATCH --mem=32G

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCa/Minor4/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Minor4 Inf microbenchmark/CCa/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCl/Minor4/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Minor4 Inf microbenchmark/CCl/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/DP1f/Minor4/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Minor4 Inf microbenchmark/DP1f/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/ED1/Minor4/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Minor4 Inf microbenchmark/ED1/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/EI/Minor4/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Minor4 Inf microbenchmark/EI/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/MI/Minor4/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Minor4 Inf microbenchmark/MI/bench.X86