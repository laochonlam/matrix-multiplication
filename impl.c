// #define A(i,j) *(matA + i * colA + j)
// #define B(i,j) *(matB + i * colB + j)
// #define C(i,j) *(matC + i * colB + j)
#define M(i,j,mat,col)  *((mat) + (i) * (col) + (j))

// for AVX
#include <immintrin.h>
#include <stdlib.h>
#include <string.h>

int *transpose_native_parallel_multiple(int *matA, int *matB, int rowA, int colA, int colB);
int *native_parallel_multiple(int *matA, int *matB, int rowA, int colA, int colB);

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

#ifdef STRASSENS_TRANSPOSE
    if(n<=8) {
        int *D = native_parallel_multiple(A, B, n, n, n);
        memcpy(C, D, n*n*sizeof(int)); 
        return;
    }

    if(n<=64) {
        int *D = transpose_native_parallel_multiple(A, B, n, n, n);
        memcpy(C, D, n * n * sizeof(int));
        return;
    }
#endif
    int m = n/2;
    int *a11, *a12, *a21, *a22;
    int *b11, *b12, *b21, *b22;
    int *c11, *c12, *c21, *c22;
    int *p1, *p2, *p3, *p4, *p5, *p6, *p7;
    int *temp1, *temp2, *temp3, *temp4, *temp5, *temp6, *temp7, *temp8, *temp9, *temp10;

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
        temp3 = malloc_2Darray(m);
        temp4 = malloc_2Darray(m);
        temp5 = malloc_2Darray(m);
        temp6 = malloc_2Darray(m);
        temp7 = malloc_2Darray(m);
        temp8 = malloc_2Darray(m);
        temp9 = malloc_2Darray(m);
        temp10 = malloc_2Darray(m);
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

    // calculate p1 ~ p7
    #pragma omp parallel sections
    {
#pragma omp section
        {
        add_matrix(a11, a22, temp1, m);
        add_matrix(b11, b22, temp2, m);
        Strassen_Matrix(temp1, temp2, p1, m);
        }

#pragma omp section
        {
        add_matrix(a21, a22, temp3, m);
        Strassen_Matrix(temp3, b11, p2, m);
        }

#pragma omp section
        {
        sub_matrix(b12, b22, temp4, m),
        Strassen_Matrix(a11, temp4, p3, m);
        }

#pragma omp section
        {
        sub_matrix(b21, b11, temp5, m),
        Strassen_Matrix(a22, temp5, p4, m);
        }

#pragma omp section
        {
        add_matrix(a11, a12, temp6, m),
        Strassen_Matrix(temp6, b22, p5, m);
        }

#pragma omp section
        {
        sub_matrix(a21, a11, temp7, m);
        add_matrix(b11, b12, temp8, m);
        Strassen_Matrix(temp7, temp8, p6, m);
        }
#pragma omp section
        {
        sub_matrix(a12, a22, temp9, m);
        add_matrix(b21, b22, temp10, m);
        Strassen_Matrix(temp9, temp10, p7, m);
        }
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
        free_2Darray(temp3);
        free_2Darray(temp4);
        free_2Darray(temp5);
        free_2Darray(temp6);
        free_2Darray(temp7);
        free_2Darray(temp8);
        free_2Darray(temp9);
        free_2Darray(temp10);
    }
}

int* native_parallel_multiple(int* matA, int* matB, int rowA, int colA, int colB)
{   
    int *matC = (int *) malloc(sizeof(int) * rowA * colB);
    #pragma omp parallel for  
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

int* strassens_parallel_multiple(int* matA, int* matB, int aligned_max)
{
    int* matC = (int*)malloc(sizeof(int) * aligned_max * aligned_max);
    Strassen_Matrix(matA, matB, matC, aligned_max);
    free_2Darray(matA);
    free_2Darray(matB);
    return matC;
}

int* transpose_native_parallel_multiple(int* matA, int* matB, int rowA, int colA, int colB)
{
    __m256i I0, I1, I3, I4, I5, I6, I7, T1, T2, T0, T3, T4, T5, T6, T7, I2;

    int* matD = (int*)malloc(sizeof(int) * colA * colB);
    for (int x = 0; x < rowA; x += 8) {
        for (int y = 0; y < colB; y += 8) {
    #define PFDISTHUGE 16
            _mm_prefetch(matB + (y + PFDISTHUGE + 0) * rowA + x, _MM_HINT_T0);
            _mm_prefetch(matB + (y + PFDISTHUGE + 1) * rowA + x, _MM_HINT_T0);
            _mm_prefetch(matB + (y + PFDISTHUGE + 2) * rowA + x, _MM_HINT_T0);
            _mm_prefetch(matB + (y + PFDISTHUGE + 3) * rowA + x, _MM_HINT_T0);
            _mm_prefetch(matB + (y + PFDISTHUGE + 4) * rowA + x, _MM_HINT_T0);
            _mm_prefetch(matB + (y + PFDISTHUGE + 5) * rowA + x, _MM_HINT_T0);
            _mm_prefetch(matB + (y + PFDISTHUGE + 6) * rowA + x, _MM_HINT_T0);
            _mm_prefetch(matB + (y + PFDISTHUGE + 7) * rowA + x, _MM_HINT_T0);

            I0 = _mm256_loadu_si256((__m256i*)(matB + (y + 0) * rowA + x));
            I1 = _mm256_loadu_si256((__m256i*)(matB + (y + 1) * rowA + x));
            I2 = _mm256_loadu_si256((__m256i*)(matB + (y + 2) * rowA + x));
            I3 = _mm256_loadu_si256((__m256i*)(matB + (y + 3) * rowA + x));
            I4 = _mm256_loadu_si256((__m256i*)(matB + (y + 4) * rowA + x));
            I5 = _mm256_loadu_si256((__m256i*)(matB + (y + 5) * rowA + x));
            I6 = _mm256_loadu_si256((__m256i*)(matB + (y + 6) * rowA + x));
            I7 = _mm256_loadu_si256((__m256i*)(matB + (y + 7) * rowA + x));

            T0 = _mm256_unpacklo_epi32(I0, I1);
            T1 = _mm256_unpackhi_epi32(I0, I1);
            T2 = _mm256_unpacklo_epi32(I2, I3);
            T3 = _mm256_unpackhi_epi32(I2, I3);
            T4 = _mm256_unpacklo_epi32(I4, I5);
            T5 = _mm256_unpackhi_epi32(I4, I5);
            T6 = _mm256_unpacklo_epi32(I6, I7);
            T7 = _mm256_unpackhi_epi32(I6, I7);

            I0 = _mm256_unpacklo_epi64(T0, T2);
            I1 = _mm256_unpackhi_epi64(T0, T2);
            I2 = _mm256_unpacklo_epi64(T1, T3);
            I3 = _mm256_unpackhi_epi64(T1, T3);
            I4 = _mm256_unpacklo_epi64(T4, T6);
            I5 = _mm256_unpackhi_epi64(T4, T6);
            I6 = _mm256_unpacklo_epi64(T5, T7);
            I7 = _mm256_unpackhi_epi64(T5, T7);

            T0 = _mm256_permute2x128_si256(I0, I4, 0x20);
            T1 = _mm256_permute2x128_si256(I1, I5, 0x20);
            T2 = _mm256_permute2x128_si256(I2, I6, 0x20);
            T3 = _mm256_permute2x128_si256(I3, I7, 0x20);
            T4 = _mm256_permute2x128_si256(I0, I4, 0x31);
            T5 = _mm256_permute2x128_si256(I1, I5, 0x31);
            T6 = _mm256_permute2x128_si256(I2, I6, 0x31);
            T7 = _mm256_permute2x128_si256(I3, I7, 0x31);

            _mm256_storeu_si256((__m256i*)(matD + ((x + 0) * colB) + y), T0);
            _mm256_storeu_si256((__m256i*)(matD + ((x + 1) * colB) + y), T1);
            _mm256_storeu_si256((__m256i*)(matD + ((x + 2) * colB) + y), T2);
            _mm256_storeu_si256((__m256i*)(matD + ((x + 3) * colB) + y), T3);
            _mm256_storeu_si256((__m256i*)(matD + ((x + 4) * colB) + y), T4);
            _mm256_storeu_si256((__m256i*)(matD + ((x + 5) * colB) + y), T5);
            _mm256_storeu_si256((__m256i*)(matD + ((x + 6) * colB) + y), T6);
            _mm256_storeu_si256((__m256i*)(matD + ((x + 7) * colB) + y), T7);
        }
    }

    int* matC = (int*)malloc(sizeof(int) * rowA * colB);
#pragma omp parallel for 
    for (int i = 0; i < rowA; ++i) {
        for (int j = 0; j < colA; ++j) {
            int sum = 0;
            for (int k = 0; k < colB; ++k) {
                sum += M(i, k, matA, colA) * M(j, k, matD, colB);
            }
            M(i, j, matC, colB) = sum;
        }
    }
    return matC;
}