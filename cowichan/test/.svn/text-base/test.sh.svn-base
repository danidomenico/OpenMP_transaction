#!/bin/sh

TOLERANCE=0.001
BIN=$1
shift
TESTS=$@

if [ "$TESTS" = "" ]; then
    TESTS="gauss half hull invperc life mandel norm outer sor 
           thresh vecdiff winnow"
fi

echo Tests: $TESTS
EXTRA_BIN=../haskell/cowichan_seq

SUCCESS=0
FAILURE=0

CURRENT_PASS=1

mat_diff() {
    RES=`$EXTRA_BIN mateq --type $1 --m1 latest/$2 --m2 baselines/$2`
    if [ "$RES" = "False" ]; then
	echo " * Baselines differ at $2"
	CURRENT_PASS=0
    fi
}

vec_diff() {
    RES=`$EXTRA_BIN veceq --type $1 --v1 latest/$2 --v2 baselines/$2`
    if [ "$RES" = "False" ]; then
	echo " * Baselines differ at $2"
	CURRENT_PASS=0
    fi
}

float_diff() {
# there's probably a nicer way to do this...
    F1=`cat latest/$1`
    F2=`cat baselines/$1`
    EQ=`echo "a=$F1-$F2;if(a<0){a > -$TOLERANCE}else{a < $TOLERANCE}" | bc -l`
    if [ "$EQ" = "0"  ]; then
	echo " * Baselines differ at $1"
	CURRENT_PASS=0
    fi
}

gauss_test() {
    CMD="gauss --matrix $2 --target $3 --out latest/$1"
    $BIN $CMD
    vec_diff float $1
}

half_test() {
    CMD="half --in $2 --out latest/$1"
    $BIN $CMD
    mat_diff int $1
}

hull_test() {
    CMD="hull --in $2 --out latest/$1"
    $BIN $CMD
    vec_diff point $1
}

invperc_test() {
    CMD="invperc --in $2 --fill $3 --out latest/$1"
    $BIN $CMD
    mat_diff bool $1
}

life_test() {
    CMD="life --in $2 --numgens $3 --out latest/$1"
    $BIN $CMD
    mat_diff bool $1
}

mandel_test() {
    CMD="mandel --x $2 --y $3 --dx $4 --dy $5 --rows $6 --cols $7 --out \
         latest/$1"
    $BIN $CMD
    mat_diff int $1
}

norm_test() {
    CMD="norm --in $2 --out latest/$1"
    $BIN $CMD
    vec_diff point $1
}

outer_test() {
    CMD="outer --in $2 --mout latest/$1_mat --vout latest/$1_vec"
    $BIN $CMD
    vec_diff float $1_vec
    mat_diff float $1_mat
}

sor_test() {
    CMD="sor --matrix $2 --target $3 --out latest/$1"
    if [ "$4" != "" ]; then
	CMD="$CMD --tolerance $4"
    fi
    $BIN $CMD
    vec_diff float $1
}

thresh_test() {
    CMD="thresh --in $2 --percent $3 --out latest/$1"
    $BIN $CMD
    mat_diff bool $1
}

vecdiff_test() {
    CMD="vecdiff --v1 $2 --v2 $3 --out latest/$1"
    $BIN $CMD
    float_diff $1
}

winnow_test() {
    CMD="winnow --matrix $2 --mask $3 --nelts $4 --out latest/$1"
    $BIN $CMD
#    vec_diff point $1
}

echo Running tests for $BIN

gauss_tests() {
    gauss_test gauss1 data/mat_1x1_float_sdd data/vec_1_float
    gauss_test gauss2 data/mat_5x5_float_sdd data/vec_5_float
    gauss_test gauss3 data/mat_10x10_float_sdd data/vec_10_float
    gauss_test gauss4 data/mat_50x50_float_sdd data/vec_50_float
}

half_tests() {
    half_test half1 data/mat_1x1_int
    half_test half2 data/mat_5x5_int
    half_test half3 data/mat_10x10_int
    half_test half4 data/mat_10x50_int
    half_test half5 data/mat_50x10_int
    half_test half6 data/mat_50x50_int
}

hull_tests() {
    hull_test hull1 data/vec_1_point
    hull_test hull2 data/vec_5_point
    hull_test hull3 data/vec_10_point
    hull_test hull4 data/vec_50_point
}

invperc_tests() {
    invperc_test invperc1 data/mat_5x5_nat 10
    invperc_test invperc2 data/mat_10x10_nat 20
#    invperc_test invperc3 data/mat_10x50_int 50
#    invperc_test invperc4 data/mat_50x10_int 75
    invperc_test invperc3 data/mat_50x50_nat 50
    invperc_test invperc4 data/mat_50x50_nat 75
    invperc_test invperc5 data/mat_50x50_nat 100
}

life_tests() {
    life_test life1 data/mat_5x5_bool 10
    life_test life2 data/mat_10x10_bool 15
    life_test life3 data/mat_10x50_bool 20
    life_test life4 data/mat_50x10_bool 25
    life_test life5 data/mat_50x50_bool 30
    life_test life6 data/mat_50x50_bool 35
}

mandel_tests() {
    mandel_test mandel1 0 0 1 1 5 5
    mandel_test mandel2 0 0 1 1 10 10
    mandel_test mandel3 -1 -1 1 1 10 50
    mandel_test mandel4 -1 -1 1 1 50 10
    mandel_test mandel5 -5 -1 10 2 50 50
}

norm_tests() {
    norm_test norm1 data/vec_5_point
    norm_test norm2 data/vec_10_point
    norm_test norm3 data/vec_50_point
}

outer_tests() {
#    outer_test outer1 data/vec_1_point
    outer_test outer1 data/vec_5_point
    outer_test outer2 data/vec_10_point
    outer_test outer3 data/vec_50_point
}


sor_tests() {
    sor_test sor1 data/mat_1x1_float_sdd data/vec_1_float 0.0001
    sor_test sor2 data/mat_5x5_float_sdd data/vec_5_float 0.0001
    sor_test sor3 data/mat_10x10_float_sdd data/vec_10_float 0.0001
    sor_test sor4 data/mat_50x50_float_sdd data/vec_50_float 0.0001
}

thresh_tests() {
    thresh_test thresh1 data/mat_5x5_int 0.5
    thresh_test thresh2 data/mat_10x10_int 0.4
    thresh_test thresh3 data/mat_10x50_int 0.3
    thresh_test thresh4 data/mat_50x10_int 0.3
    thresh_test thresh5 data/mat_50x50_int 0.2
    thresh_test thresh6 data/mat_50x50_int 0.75
}

vecdiff_tests() {
    vecdiff_test vecdiff1 data/vec_1_float data/vec_1_float2
    vecdiff_test vecdiff2 data/vec_5_float data/vec_5_float2
    vecdiff_test vecdiff3 data/vec_10_float data/vec_10_float2
    vecdiff_test vecdiff4 data/vec_50_float data/vec_50_float2
}

winnow_tests() {
    winnow_test winnow1 data/mat_5x5_int data/mat_5x5_bool 10
    winnow_test winnow2 data/mat_10x10_int data/mat_10x10_bool 25
    winnow_test winnow3 data/mat_10x50_int data/mat_10x50_bool 75
    winnow_test winnow4 data/mat_50x10_int data/mat_50x10_bool 100
    winnow_test winnow5 data/mat_50x50_int data/mat_50x50_bool 100
    winnow_test winnow6 data/mat_50x50_int data/mat_50x50_bool 200
}

for test in $TESTS; do
    `echo $test _tests | sed 's/ //'`
done
