#define A(i,j) (matA + i * colA + j)
#define B(i,j) (matB + i * colB + j)
#define C(i,j) (matC + i * colB + j)

int* native_parallel_multiple(int* matA, int* matB, int rowA, int colA, int colB)
{   
    int *matC = (int *) malloc(sizeof(int) * rowA * colB);
    for(int i=0; i<rowA; ++i) {
        for(int j=0; j<colA; ++j) {
            int sum = 0;
            for(int k=0; k<colB; ++k)
                sum += *A(i, k)*(*B(k, j));
            *C(i, j) = sum;
        }
    }
    return matC;
}
void strassens_parallel_multiple(int* matA, int* matB)
{
}