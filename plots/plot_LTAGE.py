# Set the absolute path in 

import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import os

if not os.getenv("PROJECT_DIR"):
    print("Set Current Directory (PROJECT_DIR)\n")
    exit(1)


datadir = os.getenv("PROJECT_DIR") + '/results/X86/run_ltage'

def gem5GetStat(filename, stat):
    filename = os.path.join(datadir, '', filename, 'stats.txt').replace('\\','/')
    with open(filename) as f:
        r = f.read()
        if len(r) < 10: return 0.0
        if (r.find(stat) != -1) :
            start = r.find(stat) + len(stat) + 1
            end = r.find('#', start)
            print(r[start:end])
            return float(r[start:end])
        else:
            return float(0.0)
all_arch = ['X86']
plt_arch = ['X86']


all_memory_models = ['SingleCycle', 'Inf', 'Slow']
plt_memory_models = ['Slow']

all_gem5_cpus = ['Simple','DefaultO3','Minor4', 'O3_W2K', 'O3_W256']
plt_gem5_cpus = ['Simple', 'O3_W2K', 'DefaultO3', 'O3_W256']

benchmarks = ['CCa',   'CCl',   'DP1f',  'ED1',  'EI', 'MI']

rows = []
for bm in benchmarks: 
    for cpu in plt_gem5_cpus:
        for mem in plt_memory_models:
            rows.append([bm,cpu,mem,
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'system.cpu.numCycles'),
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'sim_insts'),
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'sim_ops'),
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'sim_ticks')/1e9,
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'host_op_rate'),
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'system.cpu.BranchMispred'),
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'system.cpu.commit.branchMispredicts'),
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'system.cpu.Branches'),
                gem5GetStat(datadir+"/"+bm+"/"+cpu+"/"+mem, 'system.cpu.branchPred.lookups'),
            ])

df = pd.DataFrame(rows, columns=['benchmark','cpu', 'mem', 'cycles','instructions', 'Ops', 'Ticks','Host', 'Mispred','O3_W2K_Mispred', 'Branches', 'O3_W2K_Branches'])
df['ipc'] = df['instructions']/df['cycles']
df['cpi']= 1/df['ipc']
print(df)

def plot_O3_W2K():
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    
    d = df[(df['cpu'] == 'O3_W2K')]
    ax.bar(np.arange(len(benchmarks)), d['O3_W2K_Mispred'], width=.4, label="Mispredictions")
    ax.bar(np.arange(len(benchmarks)) + .4, d['O3_W2K_Branches'], width=.4, label="Total Predictions")
    
    ax.set_yscale('log')
    ax.set_xticks(np.arange(len(benchmarks)) + .2, labels=benchmarks)
    
    plt.legend(loc=1, prop={'size': 8})
    plt.title("LTAGE Branch Predictions vs Benchmark (O3_W2K)")

    plt.savefig("fig1-L.png", format='png', dpi=600)

def plot_O3_W256():
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)

    d = df[(df['cpu'] == 'O3_W256')]
    ax.bar(np.arange(len(benchmarks)), d['O3_W2K_Mispred'], width=.4, label="Mispredictions")
    ax.bar(np.arange(len(benchmarks)) + .4, d['O3_W2K_Branches'], width=.4, label="Total Predictions")

    ax.set_yscale('log')
    ax.set_xticks(np.arange(len(benchmarks)) + .2, labels=benchmarks)
    
    plt.legend(loc=1, prop={'size': 8})
    plt.title("LTAGE Branch Predictions vs Benchmark (O3_W256)")

    plt.savefig("fig2-L.png", format='png', dpi=600)
    
def plot_Simple():
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)

    d = df[(df['cpu'] == 'Simple')]
    ax.bar(np.arange(len(benchmarks)), d['Mispred'], width=.4, label="Mispredictions")
    ax.bar(np.arange(len(benchmarks)) + .4, d['Branches'], width=.4, label="Total Predictions")
    
    ax.set_yscale('log')
    ax.set_xticks(np.arange(len(benchmarks)) + .2, labels=benchmarks)
    
    plt.legend(loc=1, prop={'size': 8})
    plt.title("LTAGE Branch Predictions vs Benchmark (Simple)")

    plt.savefig("fig3-L.png", format='png', dpi=600)

def plot_DefaultO3():
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)

    d = df[(df['cpu'] == 'DefaultO3')]
    ax.bar(np.arange(len(benchmarks)), d['O3_W2K_Mispred'], width=.4, label="Mispredictions")
    ax.bar(np.arange(len(benchmarks)) + .4, d['O3_W2K_Branches'], width=.4, label="Total Predictions")

    ax.set_yscale('log')
    ax.set_xticks(np.arange(len(benchmarks)) + .2, labels=benchmarks)
    
    plt.legend(loc=1, prop={'size': 8})
    plt.title("LTAGE Branch Predictions vs Benchmark (DefaultO3)")

    plt.savefig("fig4-L.png", format='png', dpi=600)



plot_O3_W2K()
plot_O3_W256()
plot_Simple()
plot_DefaultO3()