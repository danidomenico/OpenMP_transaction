CFLAGS = -g -Wall

APP = chain
CC = mpiCC
LIBS = -L/h/u4/c8/03/c8borzen/494/boost_mpi/lib -lboost_mpi-gcc41-mt-1_37 -lboost_serialization-gcc41-mt-1_37
INCLUDES = -I/h/u4/c8/03/c8borzen/494/boost_mpi/include/boost-1_37
OBJS = ${APP}.o gauss_serial.o gauss_parallel.o half_serial.o half_parallel.o hull_serial.o hull_parallel.o invperc_serial.o invperc_parallel.o life_serial.o life_parallel.o mandel_serial.o mandel_parallel.o norm_serial.o norm_parallel.o outer_serial.o outer_parallel.o product_serial.o product_parallel.o randmat_serial.o randmat_parallel.o sor_serial.o sor_parallel.o thresh_serial.o thresh_parallel.o vecdiff_serial.o vecdiff_parallel.o winnow_serial.o winnow_parallel.o util.o
SRCS = ${APP}.cpp ../gauss/serial.cpp ../gauss/parallel.cpp ../half/serial.cpp ../half/parallel.cpp ../hull/serial.cpp ../hull/parallel.cpp ../invperc/serial.cpp ../invperc/parallel.cpp ../life/serial.cpp ../life/parallel.cpp ../mandel/serial.cpp ../mandel/parallel.cpp ../norm/serial.cpp ../norm/parallel.cpp ../outer/serial.cpp ../outer/parallel.cpp ../product/serial.cpp ../product/parallel.cpp ../randmat/serial.cpp ../randmat/parallel.cpp ../sor/serial.cpp ../sor/parallel.cpp ../thresh/serial.cpp ../thresh/parallel.cpp ../vecdiff/serial.cpp ../vecdiff/parallel.cpp ../winnow/serial.cpp ../winnow/parallel.cpp ../shared/util.cpp
HDRS =  ../gauss/serial.h ../gauss/parallel.h ../half/serial.h ../half/parallel.h ../hull/serial.h ../hull/parallel.h ../invperc/serial.h ../invperc/parallel.h ../life/serial.h ../life/parallel.h ../mandel/serial.h ../mandel/parallel.h ../norm/serial.h ../norm/parallel.h ../outer/serial.h ../outer/parallel.h ../product/serial.h ../product/parallel.h ../randmat/serial.h ../randmat/parallel.h ../sor/serial.h ../sor/parallel.h ../thresh/serial.h ../thresh/parallel.h ../vecdiff/serial.h ../vecdiff/parallel.h ../winnow/serial.h ../winnow/parallel.h ../include/main.h ../include/util.h ../include/generic.h ../include/type.h

all: chain

# The variable $@ has the value of the target.
chain: ${OBJS}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

chain.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -c chain.cpp ${LIBS}

gauss_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o gauss_serial.o -c ../gauss/serial.cpp ${LIBS}

gauss_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o gauss_parallel.o -c ../gauss/parallel.cpp ${LIBS}

half_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o half_serial.o -c ../half/serial.cpp ${LIBS}

half_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o half_parallel.o -c ../half/parallel.cpp ${LIBS}

hull_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o hull_serial.o -c ../hull/serial.cpp ${LIBS}

hull_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o hull_parallel.o -c ../hull/parallel.cpp ${LIBS}

invperc_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o invperc_serial.o -c ../invperc/serial.cpp ${LIBS}

invperc_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o invperc_parallel.o -c ../invperc/parallel.cpp ${LIBS}

life_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o life_serial.o -c ../life/serial.cpp ${LIBS}

life_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o life_parallel.o -c ../life/parallel.cpp ${LIBS}

mandel_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o mandel_serial.o -c ../mandel/serial.cpp ${LIBS}

mandel_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o mandel_parallel.o -c ../mandel/parallel.cpp ${LIBS}

norm_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o norm_serial.o -c ../norm/serial.cpp ${LIBS}

norm_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o norm_parallel.o -c ../norm/parallel.cpp ${LIBS}

outer_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o outer_serial.o -c ../outer/serial.cpp ${LIBS}

outer_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o outer_parallel.o -c ../outer/parallel.cpp ${LIBS}

product_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o product_serial.o -c ../product/serial.cpp ${LIBS}

product_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o product_parallel.o -c ../product/parallel.cpp ${LIBS}

randmat_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o randmat_serial.o -c ../randmat/serial.cpp ${LIBS}

randmat_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o randmat_parallel.o -c ../randmat/parallel.cpp ${LIBS}

sor_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o sor_serial.o -c ../sor/serial.cpp ${LIBS}

sor_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o sor_parallel.o -c ../sor/parallel.cpp ${LIBS}

thresh_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o thresh_serial.o -c ../thresh/serial.cpp ${LIBS}

thresh_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o thresh_parallel.o -c ../thresh/parallel.cpp ${LIBS}

vecdiff_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o vecdiff_serial.o -c ../vecdiff/serial.cpp ${LIBS}

vecdiff_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o vecdiff_parallel.o -c ../vecdiff/parallel.cpp ${LIBS}

winnow_serial.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o winnow_serial.o -c ../winnow/serial.cpp ${LIBS}

winnow_parallel.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -o winnow_parallel.o -c ../winnow/parallel.cpp ${LIBS}

util.o: ${SRCS} ${HDRS}
	${CC} ${CFLAGS} ${INCLUDES} -c ../shared/util.cpp ${LIBS}

clean:
	rm *.o ${APP}
