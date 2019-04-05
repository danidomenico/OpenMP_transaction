#!/bin/sh
g++ -Wall -m64 -O2 -D LIN64 -c ../cowichan/cowichan.cpp
g++ -Wall -m64 -O2 -D LIN64 -o cowichan_serial *.cpp cowichan.o
