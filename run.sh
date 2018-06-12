PURPLE="\033[0;35m"
ORANGE="\033[0;33m"
BLUE="\033[0;34m"
CYAN="\033[0;36m"
RED="\033[0;31m"
NORMAL="\033[0m"

test1=test_data/test1
echo ${PURPLE}[test1]${NORMAL}
echo ${ORANGE}native_parallel_multiplication${NORMAL}
./native_parallel_multiplication < ${test1}
echo ${CYAN}transpose_native_parallel_multiplication${NORMAL}
./transpose_native_parallel_multiplication < ${test1}
echo ${RED}strassens_parallel_multiplication${NORMAL}
./strassens_parallel_multiplication < ${test1}
echo ${BLUE}transpose_strassens_parallel_multiplication${NORMAL}
./transpose_strassens_parallel_multiplication < ${test1}

echo diff native_parallel_multiplication.output transpose_native_parallel_multiplication.output
diff native_parallel_multiplication.output transpose_native_parallel_multiplication.output
echo diff transpose_native_parallel_multiplication.output strassens_parallel_multiplication.output
diff transpose_native_parallel_multiplication.output strassens_parallel_multiplication.output
echo diff strassens_parallel_multiplication.output transpose_strassens_parallel_multiplication.output
diff strassens_parallel_multiplication.output transpose_strassens_parallel_multiplication.output
echo diff transpose_strassens_parallel_multiplication.output native_parallel_multiplication.output
diff transpose_strassens_parallel_multiplication.output native_parallel_multiplication.output

test2=test_data/test2
echo 
echo ${PURPLE}[test2]${NORMAL}
echo ${ORANGE}native_parallel_multiplication${NORMAL}
./native_parallel_multiplication < ${test2}
echo ${CYAN}transpose_native_parallel_multiplication${NORMAL}
./transpose_native_parallel_multiplication < ${test2}
echo ${RED}strassens_parallel_multiplication${NORMAL}
./strassens_parallel_multiplication < ${test2}
echo ${BLUE}transpose_strassens_parallel_multiplication${NORMAL}
./transpose_strassens_parallel_multiplication < ${test2}

echo diff native_parallel_multiplication.output transpose_native_parallel_multiplication.output
diff native_parallel_multiplication.output transpose_native_parallel_multiplication.output
echo diff transpose_native_parallel_multiplication.output strassens_parallel_multiplication.output
diff transpose_native_parallel_multiplication.output strassens_parallel_multiplication.output
echo diff strassens_parallel_multiplication.output transpose_strassens_parallel_multiplication.output
diff strassens_parallel_multiplication.output transpose_strassens_parallel_multiplication.output
echo diff transpose_strassens_parallel_multiplication.output native_parallel_multiplication.output
diff transpose_strassens_parallel_multiplication.output native_parallel_multiplication.output

test3=test_data/test3
echo 
echo ${PURPLE}[test3]${NORMAL}
echo ${ORANGE}native_parallel_multiplication${NORMAL}
./native_parallel_multiplication < ${test3}
echo ${CYAN}transpose_native_parallel_multiplication${NORMAL}
./transpose_native_parallel_multiplication < ${test3}
echo ${RED}strassens_parallel_multiplication${NORMAL}
./strassens_parallel_multiplication < ${test3}
echo ${BLUE}transpose_strassens_parallel_multiplication${NORMAL}
./transpose_strassens_parallel_multiplication < ${test3}

echo diff native_parallel_multiplication.output transpose_native_parallel_multiplication.output
diff native_parallel_multiplication.output transpose_native_parallel_multiplication.output
echo diff transpose_native_parallel_multiplication.output strassens_parallel_multiplication.output
diff transpose_native_parallel_multiplication.output strassens_parallel_multiplication.output
echo diff strassens_parallel_multiplication.output transpose_strassens_parallel_multiplication.output
diff strassens_parallel_multiplication.output transpose_strassens_parallel_multiplication.output
echo diff transpose_strassens_parallel_multiplication.output native_parallel_multiplication.output
diff transpose_strassens_parallel_multiplication.output native_parallel_multiplication.output