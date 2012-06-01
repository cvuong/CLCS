#!/usr/bin/env bash

if [ $# -ne 1 ]; then
  echo "Usage: ./runsample.sh CPPSOURCEFILE"
  exit
fi

echo "Compiling and running the C++ file $1..."
g++ -O2 $1 && time ./a.out < test.txt | ./judge sample.out
