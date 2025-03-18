#include <mpi.h>
#include <stdio.h>

#define N 4  

int main(int argc, char** argv) {
    int rank, size;
    int matrix[N][N], transposed[N][N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                matrix[i][j] = i * N + j + 1;
    }

    MPI_Bcast(matrix, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = rank; i < N; i += size)
        for (int j = 0; j < N; j++)
            transposed[j][i] = matrix[i][j];

    MPI_Gather(transposed, N * N / size, MPI_INT, transposed, N * N / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Transposed Matrix:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                printf("%d ", transposed[i][j]);
            printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}

