#!/bin/sh
# not tested
g++ -Wall -m32 -ltbb -O2 -D LIN32 -c ../cowichan/cowichan.cpp
g++ -Wall -m32 -ltbb -O2 -D LIN32 -o cowichan_tbb *.cpp cowichan.o
