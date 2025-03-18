#include <mpi.h>
#include <stdio.h>

#define N 8

int main(int argc, char** argv) {
    int rank, size;
    int arr[N], prefix_sum[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            arr[i] = i + 1;  
    }

    MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i <= rank; i++)
        sum += arr[i];

    MPI_Scan(&sum, &prefix_sum[rank], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank == size - 1) {
        printf("Prefix Sum: ");
        for (int i = 0; i < N; i++)
            printf("%d ", prefix_sum[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

