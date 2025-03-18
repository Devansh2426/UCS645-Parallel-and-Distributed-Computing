#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 70

void multiply_row(int row, int A[N][N], int B[N][N], int C[N][N]) {
    for (int j = 0; j < N; j++) {
        C[row][j] = 0;
        for (int k = 0; k < N; k++)
            C[row][j] += A[row][k] * B[k][j];
    }
}

int main(int argc, char** argv) {
    int rank, size, A[N][N], B[N][N], C[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                A[i][j] = rand() % 10;
                B[i][j] = rand() % 10;
            }
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = rank; i < N; i += size)
        multiply_row(i, A, B, C);

    MPI_Gather(C, N * N / size, MPI_INT, C, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Multiplication Done.\n");
    }

    MPI_Finalize();
    return 0;
}

