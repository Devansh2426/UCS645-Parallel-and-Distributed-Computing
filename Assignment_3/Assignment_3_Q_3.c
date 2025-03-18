#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM 100

int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main(int argc, char** argv) {
    int rank, size, num, flag;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (num = 2; num <= MAX_NUM; num++) {
            int worker;
            MPI_Recv(&worker, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&num, 1, MPI_INT, worker, 0, MPI_COMM_WORLD);
        }

        for (int i = 1; i < size; i++) {
            int stop_signal = -1;
            MPI_Recv(&flag, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Send(&stop_signal, 1, MPI_INT, flag, 0, MPI_COMM_WORLD);
        }
    } else {
        while (1) {
            int test_number;
            MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Recv(&test_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

            if (test_number == -1) break;

            flag = is_prime(test_number) ? test_number : -test_number;
            MPI_Send(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0) {
        printf("Prime numbers up to %d:\n", MAX_NUM);
        for (num = 2; num <= MAX_NUM; num++) {
            MPI_Recv(&flag, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            if (flag > 0) printf("%d ", flag);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

