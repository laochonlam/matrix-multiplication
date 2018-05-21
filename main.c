#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "impl.c"

int main(int argc, char **argv){

    int rowA, colA, rowB, colB;

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

    
#ifdef NATIVE_PARALLEL
    int* result = native_parallel_multiple(matA, matB, rowA, colA, colB);
    for (int i = 0; i < rowA; i++){
        for (int j = 0; j < colB; j++){
            printf("%d",*(result + i * colB + j));
            if(j==colB-1){
                if(i!=rowA-1)printf("\n");
            }
            else printf(" ");
        }
    }
        

#endif

#ifdef STRASSENS_PARALLEL
    strassens_parallel_multiple(matA, matB);
#endif

}