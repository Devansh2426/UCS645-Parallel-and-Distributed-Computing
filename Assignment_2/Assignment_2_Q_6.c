#include <mpi.h>
#include <stdio.h>

#define N 10

int main(int argc, char** argv) {
    int rank, size;
    int A[N], B[N], local_sum = 0, global_sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            A[i] = B[i] = i + 1;
    }

    MPI_Bcast(A, N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = rank; i < N; i += size)
        local_sum += A[i] * B[i];

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Dot Product: %d\n", global_sum);

    MPI_Finalize();
    return 0;
}

