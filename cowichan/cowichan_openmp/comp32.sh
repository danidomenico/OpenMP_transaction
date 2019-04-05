#!/bin/sh
# not tested
g++ -Wall -m32 -fopenmp -D LIN32 -O2 -c ../cowichan/cowichan.cpp
g++ -Wall -m32 -fopenmp -D LIN32 -O2 -o cowichan_openmp *.cpp cowichan.o
