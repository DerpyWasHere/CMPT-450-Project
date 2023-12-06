#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=60:00
#SBATCH --mem=32G

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCa/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py O3_W2K Slow microbenchmark/CCa/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/CCl/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py O3_W2K Slow microbenchmark/CCl/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/DP1f/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py O3_W2K Slow microbenchmark/DP1f/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/ED1/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py O3_W2K Slow microbenchmark/ED1/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/EI/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py O3_W2K Slow microbenchmark/EI/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_micro/MI/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_micro.py O3_W2K Slow microbenchmark/MI/bench.X86

srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/CCa/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py O3_W2K Slow microbenchmark/CCa/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/CCl/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py O3_W2K Slow microbenchmark/CCl/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/DP1f/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py O3_W2K Slow microbenchmark/DP1f/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/ED1/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py O3_W2K Slow microbenchmark/ED1/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/EI/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py O3_W2K Slow microbenchmark/EI/bench.X86
srun build/X86/gem5.opt -re --outdir=$PWD/results/X86/run_ltage/MI/O3_W2K/Slow --debug-flags=Event,ExecAll --debug-file=trace.out --debug-start=0 gem5-config/run_ltage.py O3_W2K Slow microbenchmark/MI/bench.X86