#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N (1 << 16) // 2^16 elements

void daxpy_serial(double a, double *X, double *Y) {
    for (int i = 0; i < N; i++)
        X[i] = a * X[i] + Y[i];
}

void daxpy_parallel(double a, double *X, double *Y, int rank, int size) {
    int chunk_size = N / size;
    int start = rank * chunk_size;
    int end = start + chunk_size;

    for (int i = start; i < end; i++)
        X[i] = a * X[i] + Y[i];

    MPI_Gather(X + start, chunk_size, MPI_DOUBLE, X, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    int rank, size;
    double *X, *Y, a = 2.5, start_time, end_time, parallel_time, serial_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    X = (double*) malloc(N * sizeof(double));
    Y = (double*) malloc(N * sizeof(double));

    if (rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            X[i] = rand() % 100;
            Y[i] = rand() % 100;
        }
        start_time = MPI_Wtime();
        daxpy_serial(a, X, Y);
        serial_time = MPI_Wtime() - start_time;
    }

    MPI_Bcast(X, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(Y, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    start_time = MPI_Wtime();
    daxpy_parallel(a, X, Y, rank, size);
    parallel_time = MPI_Wtime() - start_time;

    if (rank == 0) {
        printf("Serial Time: %f sec\n", serial_time);
        printf("Parallel Time: %f sec\n", parallel_time);
        printf("Speedup: %f\n", serial_time / parallel_time);
    }

    free(X);
    free(Y);
    MPI_Finalize();
    return 0;
}

