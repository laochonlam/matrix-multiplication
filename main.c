#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/time.h>
#include <time.h> 
#include <math.h>
#include "impl.c"
#define max(a, b) (((a) > (b)) ? (a) : (b))

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
    int aligned_rowA, aligned_colA, aligned_rowB, aligned_colB;
    char prog_name[128];
    scanf("%d %d", &rowA, &colA);
    //align by 8
    aligned_rowA = rowA;
    aligned_colA = colA;
    if (rowA % 8 != 0)
        aligned_rowA = rowA + (8 - (rowA % 8));
    if (colA % 8 != 0)
        aligned_colA = colA + (8 - (colA % 8));

    // read Matrix A
    int* matA = (int*)malloc(sizeof(int) * aligned_rowA * aligned_colA);
    memset(matA, 0, sizeof(matA));
    for (int i = 0; i < rowA; i++)
        for (int j = 0; j < colA; j++)
            scanf("%d", (matA + i * aligned_colA + j));

    //read Matrix B
    scanf("%d %d", &rowB, &colB);
    //align by 8
    aligned_rowB = rowB;
    aligned_colB = colB;
    if (rowB % 8 != 0)
        aligned_rowB = rowB + (8 - (rowB % 8));
    if (colB % 8 != 0)
        aligned_colB = colB + (8 - (colB % 8));
    int* matB = (int*)malloc(sizeof(int) * aligned_rowB * aligned_colB);
    memset(matB, 0, sizeof(matB));
    for (int i = 0; i < rowB; i++)
        for (int j = 0; j < colB; j++)
            scanf("%d", (matB + i * aligned_colB + j));

    int* result;
    struct timespec start, end;
    // printf("%d %d %d", aligned_rowA, aligned_colA, aligned_colB);
    clock_gettime(CLOCK_REALTIME, &start);
#ifdef NATIVE_PARALLEL
    result = native_parallel_multiple(matA, matB, aligned_rowA, aligned_colA, aligned_colB);
#endif

#ifdef STRASSENS_PARALLEL
    int aligened_max = max(max(max(aligned_rowA, aligned_colA), aligned_rowB), aligned_colB);
    int* matA_aligned = (int*)malloc(sizeof(int) * aligened_max * aligened_max);
    memset(matA_aligned, 0, sizeof(matA_aligned));
    int* matB_aligned = (int*)malloc(sizeof(int) * aligened_max * aligened_max);
    memset(matB_aligned, 0, sizeof(matB_aligned));
    for (int i = 0; i < rowA; i++)
        for (int j = 0; j < colA; j++) 
            *(matA_aligned + i * aligened_max + j) = *(matA + i * aligned_colA + j);
    for (int i = 0; i < rowB; i++)
        for (int j = 0; j < colB; j++) 
            *(matB_aligned + i * aligened_max + j) = *(matB + i * aligned_colB + j);

    result = strassens_parallel_multiple(matA_aligned, matB_aligned, aligened_max);
#endif

#ifdef TRAN_NATIVE_PARALLEL
    result = transpose_native_parallel_multiple(matA, matB, aligned_rowA, aligned_colA, aligned_colB);
#endif 

    clock_gettime(CLOCK_REALTIME, &end);
    char* output = strcat(argv[0]+2, ".output");
    FILE *fp = fopen(output, "w"); 
    for (int i = 0; i < rowA; i++) {
        for (int j = 0; j < colB; j++) {
            fprintf(fp, "%d", *(result + i * aligned_colB + j));
            if (j == colB - 1) {
                if (i != rowA - 1)
                    fprintf(fp, "\n");
            } else
                fprintf(fp, " ");
        }
    } 
    fclose(fp);
    long int cpu_time = diff_in_us(start, end);
    printf("Execution Time: %ld us\n\n", cpu_time);
} 