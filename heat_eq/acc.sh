#!/bin/bash

>acc.out

for ((i=1; i<=11; i++))
do
    mpirun -n $i --hostfile ../hosts  ./term 1.0 2>>acc.out
    echo "$1 completed!"
done
