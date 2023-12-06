#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=60:00
#SBATCH --mem=32G

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCa/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py DefaultO3 Slow microbenchmark/CCa/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCl/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py DefaultO3 Slow microbenchmark/CCl/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/DP1f/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py DefaultO3 Slow microbenchmark/DP1f/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/ED1/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py DefaultO3 Slow microbenchmark/ED1/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/EI/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py DefaultO3 Slow microbenchmark/EI/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/MI/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py DefaultO3 Slow microbenchmark/MI/bench.X86

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/CCa/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py DefaultO3 Slow microbenchmark/CCa/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/CCl/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py DefaultO3 Slow microbenchmark/CCl/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/DP1f/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py DefaultO3 Slow microbenchmark/DP1f/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/ED1/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py DefaultO3 Slow microbenchmark/ED1/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/EI/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py DefaultO3 Slow microbenchmark/EI/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/MI/DefaultO3/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py DefaultO3 Slow microbenchmark/MI/bench.X86