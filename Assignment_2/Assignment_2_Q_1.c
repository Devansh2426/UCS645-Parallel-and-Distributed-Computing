#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_POINTS 1000000

int main(int argc, char** argv) {
    int rank, size, local_count = 0, global_count;
    double x, y;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);
    int points_per_process = NUM_POINTS / size;

    for (int i = 0; i < points_per_process; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1) local_count++;
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        double pi = 4.0 * global_count / NUM_POINTS;
        printf("Estimated Pi: %f\n", pi);
    }

    MPI_Finalize();
    return 0;
}

