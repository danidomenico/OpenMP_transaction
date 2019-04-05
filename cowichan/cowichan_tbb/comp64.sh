#!/bin/sh
# not tested
g++ -Wall -m64 -ltbb -O2 -D LIN64 -c ../cowichan/cowichan.cpp
g++ -Wall -m64 -ltbb -O2 -D LIN64 -o cowichan_tbb *.cpp cowichan.o
