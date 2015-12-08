#!/bin/bash

# Dieses Skript fuehrt die Simulation für alle Ersetzungsalgorithmen 
# und alle Framegroessen durch

page_sizes="8 16 32 64"
algo="CLOCK AGING FIFO"

ref_result_dir="./LogFiles_mit_SEED_2806"

# clean up result directory
rm -rf results
mkdir results

# run all simulations
for s in $page_sizes
do
for a in $algo
do
    echo "Run simulation for algo $a and page size $s"
    # delete all shared memory areas
    # ipcrm -ashm

    # compile 
    make clean
    make VMEM_ALGO=VMEM_ALGO_$a VMEM_PAGESIZE=$s

    # start memory manageer
    ./mmanage &
    mmanage_pid=$!
    sleep 1 # wait for mmange to create shared objects
    # start application and save results
    ./vmappl > results/output_${a}_${s}.txt
    mv logfile.txt results/logfile_${a}_${s}.txt 

    # compare results
    echo "=============== COMPARE results for logfile_${a}_${s}.txt =================="
    diff results/logfile_${a}_${s}.txt  ${ref_result_dir}/logfile_${a}_${s}.txt
    echo "============================================================================"

    kill -s SIGINT $mmanage_pid
done
done
