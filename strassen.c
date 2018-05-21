#include <stdio.h>
#include <stdlib.h>
#define N 8

void display(int **p, int n) {
    int i, j;
    for(i=0; i<n; ++i) {
        for(j=0; j<n; ++j)
            printf("%d ", p[i][j]);
        printf("\n");
    }

    printf("\n");
}

int** malloc_2Darray(int n) {
    int **temp, i;

    temp = malloc(n * sizeof(int *));
    for(i=0; i<n; ++i)
        temp[i] = (int *)malloc(n * sizeof(int));
    
    return temp;
}

void free_2Darray(int **A, int n) {
    if(A == NULL)
        return;
    
    int i;
    for(i=0; i<n; ++i)
        free(A[i]);

    free(A);
}

void add_matrix(int **A, int **B, int **C, int n) {
    int i, j;

    for(i=0; i<n; ++i)
        for(j=0; j<n; ++j)
            C[i][j] = A[i][j] + B[i][j];
}

void sub_matrix(int **A, int **B, int **C, int n) {
    int i, j;

    for(i=0; i<n; ++i)
        for(j=0; j<n; ++j)
            C[i][j] = A[i][j] - B[i][j];
}

void Strassen_Matrix(int **A, int **B, int **C, int n) {

    if(n==1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int m = n/2;
    int **a11, **a12, **a21, **a22;
    int **b11, **b12, **b21, **b22;
    int **c11, **c12, **c21, **c22;
    int **p1, **p2, **p3, **p4, **p5, **p6, **p7;
    int **temp1, **temp2;

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
        for(j=0; j<m; ++j) {
            a11[i][j] = A[i + 0][j + 0];
            a12[i][j] = A[i + 0][j + m];
            a21[i][j] = A[i + m][j + 0];
            a22[i][j] = A[i + m][j + m];

            b11[i][j] = B[i + 0][j + 0];
            b12[i][j] = B[i + 0][j + m];
            b21[i][j] = B[i + m][j + 0];
            b22[i][j] = B[i + m][j + m];
        }
    }

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
            C[i + 0][j + 0] = c11[i][j];
            C[i + 0][j + m] = c12[i][j];
            C[i + m][j + 0] = c21[i][j];
            C[i + m][j + m] = c22[i][j];
        }

    // free
    {
        free_2Darray(a11, m);
        free_2Darray(a12, m);
        free_2Darray(a21, m);
        free_2Darray(a22, m);
        
        free_2Darray(b11, m);
        free_2Darray(b12, m);
        free_2Darray(b21, m);
        free_2Darray(b22, m);

        free_2Darray(c11, m);
        free_2Darray(c12, m);
        free_2Darray(c21, m);
        free_2Darray(c22, m);

        free_2Darray(p1, m);
        free_2Darray(p2, m);
        free_2Darray(p3, m);
        free_2Darray(p4, m);
        free_2Darray(p5, m);
        free_2Darray(p6, m);
        free_2Darray(p7, m);

        free_2Darray(temp1, m);
        free_2Darray(temp2, m);
    }
}

int main() {

    // declare A, B, C matrix
    int **A, **B, **C;
    int i, j, k, sum;

    A = malloc_2Darray(N);
    B = malloc_2Darray(N);
    C = malloc_2Darray(N);

    // input here
    for(i=0; i<N; ++i) {
        for(j=0; j<N; ++j) {
            A[i][j] = 1 + i*N + j;
            B[i][j] = 1 + i*N + j;
        }
    }

    // native multiply
    for(i=0; i<N; ++i) {
        for(j=0; j<N; ++j) {
            sum = 0;
            for(k=0; k<N; ++k)
                sum += A[i][k]*B[k][j];
            C[i][j] = sum;
        }
    }

    display(C, N);
    Strassen_Matrix(A, B, C, N);
    display(C, N);

    free_2Darray(A, N);
    free_2Darray(B, N);
    free_2Darray(C, N);
    
}