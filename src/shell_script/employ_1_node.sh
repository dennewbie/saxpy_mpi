#!/bin/bash

if [ "$1" != "" ]; then
    echo "Starting MPI employing"
else
    echo "Insert arguments: <machinefile> <nProcessors> <executableFile> <configuration file path> <master processor ID>\n"
    exit
fi

cp $3 /home/cpd2021/
mpirun --hostfile $1 -np $2 /home/cpd2021/$3 $4 $5