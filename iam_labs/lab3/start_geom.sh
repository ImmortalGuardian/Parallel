#!/bin/bash

for ((i=1; i<=20; i++))
do
    mpirun -n $i --hostfile ./hosts ./geom 30000000 >> geom.out
done
