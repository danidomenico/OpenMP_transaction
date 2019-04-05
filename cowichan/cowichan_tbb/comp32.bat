REM not tested
g++ -Wall -m32 -ltbb -O2 -D WIN32 -c ../cowichan/cowichan.cpp
g++ -Wall -m32 -ltbb -O2 -D WIN32 -o cowichan_tbb *.cpp cowichan.o
