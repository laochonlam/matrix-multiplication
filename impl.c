// #define A(i,j) *(matA + i * colA + j)
// #define B(i,j) *(matB + i * colB + j)
// #define C(i,j) *(matC + i * colB + j)
#define M(i,j,mat,col)  *((mat) + (i) * (col) + (j))


#include <string.h>
#include <stdlib.h>

void check(int *A, int n){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", *(A + i * n + j));
        }   
        printf("\n");
    }   
    printf("\n");
}

int* malloc_2Darray(int n) {
    int *matQ = (int *) malloc(sizeof(int) * n * n);
    return matQ;
}

void free_2Darray(int *A) {
    if(A == NULL)
        return;
    
    free(A);
}

void add_matrix(int *A, int *B, int *C, int n) {
    for(int i=0; i<n*n; ++i)
        *(C + i) = *(A + i) + *(B + i);
}

void sub_matrix(int *A, int *B, int *C, int n) {
    for(int i=0; i<n*n; ++i)
        *(C + i) = *(A + i) - *(B + i);
}

void Strassen_Matrix(int *A, int *B, int *C, int n) {

    if(n==1) {
        *C = *A * (*B);
        return;
    }

    int m = n/2;
    int *a11, *a12, *a21, *a22;
    int *b11, *b12, *b21, *b22;
    int *c11, *c12, *c21, *c22;
    int *p1, *p2, *p3, *p4, *p5, *p6, *p7;
    int *temp1, *temp2;

    // just allocate (use {} to hidd)
    {
        a11 = malloc_2Darray(m);
        a12 = malloc_2Darray(m);
        a21 = malloc_2Darray(m);
        a22 = malloc_2Darray(m);
        
        b11 = malloc_2Darray(m);
        b12 = malloc_2Darray(m);
        b21 = malloc_2Darray(m);
        b22 = malloc_2Darray(m);

        c11 = malloc_2Darray(m);
        c12 = malloc_2Darray(m);
        c21 = malloc_2Darray(m);
        c22 = malloc_2Darray(m);

        p1 = malloc_2Darray(m);
        p2 = malloc_2Darray(m);
        p3 = malloc_2Darray(m);
        p4 = malloc_2Darray(m);
        p5 = malloc_2Darray(m);
        p6 = malloc_2Darray(m);
        p7 = malloc_2Darray(m);

        temp1 = malloc_2Darray(m);
        temp2 = malloc_2Darray(m);
    }

    // divide to small matrix
    int i, j;

    for(i=0; i<m; ++i) {
        for(j=0; j<m; ++j){
            M(i,j,a11,m) = M(i + 0, j + 0, A,n);
            M(i,j,a12,m) = M(i + 0, j + m, A,n);
            M(i,j,a21,m) = M(i + m, j + 0, A,n);
            M(i,j,a22,m) = M(i + m, j + m, A,n);

            M(i,j,b11,m) = M(i + 0, j + 0, B,n);
            M(i,j,b12,m) = M(i + 0, j + m, B,n);
            M(i,j,b21,m) = M(i + m, j + 0, B,n);
            M(i,j,b22,m) = M(i + m, j + m, B,n);
        }
    }
    // printf("a11\n");
    // check(a11, m);
    // printf("a12\n");
    // check(a12, m);
    // check(a21, m);
    // check(a22, m);

    // calculate p1 ~ p7 
    {
        // #pragma omp section {}
        add_matrix(a11, a22, temp1, m);
        add_matrix(b11, b22, temp2, m);
        Strassen_Matrix(temp1, temp2, p1, m);
        
        add_matrix(a21, a22, temp1, m);
        Strassen_Matrix(temp1, b11, p2, m);
        
        sub_matrix(b12, b22, temp2, m),
        Strassen_Matrix(a11, temp2, p3, m);
        
        sub_matrix(b21, b11, temp2, m),
        Strassen_Matrix(a22, temp2, p4, m);
        
        add_matrix(a11, a12, temp1, m),
        Strassen_Matrix(temp1, b22, p5, m);
        
        sub_matrix(a21, a11, temp1, m);
        add_matrix(b11, b12, temp2, m);
        Strassen_Matrix(temp1, temp2, p6, m);
        
        sub_matrix(a12, a22, temp1, m);
        add_matrix(b21, b22, temp2, m);
        Strassen_Matrix(temp1, temp2, p7, m);
    }

    // calculate c11, c12, c21, c22
    {
        add_matrix(p1, p4, temp1, m);
        sub_matrix(temp1, p5, temp2, m);
        add_matrix(temp2, p7, c11, m);   // c11 = p1 + p4 - p5 + p7

        add_matrix(p3, p5, c12, m);

        add_matrix(p2, p4, c21, m);

        sub_matrix(p1, p2, temp1, m);
        add_matrix(temp1, p3, temp2, m);
        add_matrix(temp2, p6, c22, m);
    }

    for(i=0; i<m; ++i)
        for(j=0; j<m; ++j) {
            M(i+0,j+0,C,n) = M(i,j,c11,m);
            M(i+0,j+m,C,n) = M(i,j,c12,m);
            M(i+m,j+0,C,n) = M(i,j,c21,m);
            M(i+m,j+m,C,n) = M(i,j,c22,m);
        }

    // free
    {
        free_2Darray(a11);
        free_2Darray(a12);
        free_2Darray(a21);
        free_2Darray(a22);
        
        free_2Darray(b11);
        free_2Darray(b12);
        free_2Darray(b21);
        free_2Darray(b22);

        free_2Darray(c11);
        free_2Darray(c12);
        free_2Darray(c21);
        free_2Darray(c22);

        free_2Darray(p1);
        free_2Darray(p2);
        free_2Darray(p3);
        free_2Darray(p4);
        free_2Darray(p5);
        free_2Darray(p6);
        free_2Darray(p7);

        free_2Darray(temp1);
        free_2Darray(temp2);
    }
}

int* native_parallel_multiple(int* matA, int* matB, int rowA, int colA, int colB)
{   
    int *matC = (int *) malloc(sizeof(int) * rowA * colB);
    #pragma omp parallel for num_threads(4) 
    for(int i=0; i<rowA; ++i) {
        for(int j=0; j<colA; ++j) {
            int sum = 0;

            for(int k=0; k<colB; ++k)
                sum += M(i,k,matA,colA)*M(k,j,matB,colB);
            M(i,j,matC,colB) = sum;
        }
    }
    return matC;
}

int* strassens_parallel_multiple(int* matA, int* matB, int rowA, int colA, int colB)
{
    int ttwo=0x8000;
    while(1){
        ttwo /=2;
        if(ttwo<rowA)  break;
    }   
    ttwo = ttwo*2;

    int *matC = (int *) malloc(sizeof(int) * rowA * colB);
    Strassen_Matrix(matA, matB, matC, ttwo);
    free_2Darray(matA);
    free_2Darray(matB);
    return matC;
}