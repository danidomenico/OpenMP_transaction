REM not tested
g++ -Wall -m32 -I..\..\boost\include\boost-1_37 -I"C:\Program Files (x86)\MPICH\SDK\Include" -L..\..\boost\lib -L"C:\Program Files (x86)\MPICH\SDK\Lib" -lmpich -O2 -D WIN32 -c ../cowichan/cowichan.cpp
g++ -Wall -m32 -I..\..\boost\include\boost-1_37 -I"C:\Program Files (x86)\MPICH\SDK\Include" -L..\..\boost\lib -L"C:\Program Files (x86)\MPICH\SDK\Lib" -lmpich -O2 -D WIN32 -o cowichan_mpi *.cpp cowichan.o
