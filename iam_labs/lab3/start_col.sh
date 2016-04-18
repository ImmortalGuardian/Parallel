#!/bin/bash

for ((i=2; i<=21; i++))
    do
        mpirun -n $i --hostfile ./hosts ./col 300000000 > col.out
    done
