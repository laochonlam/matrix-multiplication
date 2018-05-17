#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
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
}