#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=60:00
#SBATCH --mem=32G

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCa/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Simple Inf microbenchmark/CCa/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCl/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Simple Inf microbenchmark/CCl/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/DP1f/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Simple Inf microbenchmark/DP1f/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/ED1/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Simple Inf microbenchmark/ED1/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/EI/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Simple Inf microbenchmark/EI/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/MI/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py Simple Inf microbenchmark/MI/bench.X86

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/CCa/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py Simple Inf microbenchmark/CCa/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/CCl/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py Simple Inf microbenchmark/CCl/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/DP1f/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py Simple Inf microbenchmark/DP1f/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/ED1/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py Simple Inf microbenchmark/ED1/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/EI/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py Simple Inf microbenchmark/EI/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/MI/Simple/Inf --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py Simple Inf microbenchmark/MI/bench.X86