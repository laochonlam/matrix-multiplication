#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include "impl.c"

static long diff_in_us(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec - t1.tv_nsec < 0) {
        diff.tv_sec = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec * 1000000.0 + diff.tv_nsec / 1000.0);
}

int main(int argc, char **argv){

    int rowA, colA, rowB, colB;
    char prog_name[128];
    scanf("%d %d", &rowA, &colA);
    // read Matrix A
    int *matA = (int *) malloc(sizeof(int) * rowA * colA);
    for (int i = 0; i < rowA; i++)
        for (int j = 0; j < colA; j++)
            scanf("%d", (matA + i * colA + j));
    //read Matrix B
    scanf("%d %d", &rowB, &colB);
    int *matB = (int *) malloc(sizeof(int) * rowB * colB);
    for (int i = 0; i < rowB; i++)
        for (int j = 0; j < colB; j++)
            scanf("%d", (matB + i * colB + j));

    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
#ifdef NATIVE_PARALLEL
    native_parallel_multiple(matA, matB);
#endif

#ifdef STRASSENS_PARALLEL
    strassens_parallel_multiple(matA, matB);
#endif
    clock_gettime(CLOCK_REALTIME, &end);

    long int cpu_time = diff_in_us(start, end);
    printf("Execution Time: %ld us\n", cpu_time);
}