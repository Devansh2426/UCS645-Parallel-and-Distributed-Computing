#include <mpi.h>
#include <stdio.h>

#define N 10

void heat_distribution(double grid[N][N]) {
    double temp_grid[N][N];

    for (int iter = 0; iter < 100; iter++) {
        for (int i = 1; i < N - 1; i++)
            for (int j = 1; j < N - 1; j++)
                temp_grid[i][j] = (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1] + grid[i][j + 1]) / 4.0;
        
        for (int i = 1; i < N - 1; i++)
            for (int j = 1; j < N - 1; j++)
                grid[i][j] = temp_grid[i][j];
    }
}

int main(int argc, char** argv) {
    int rank, size;
    double grid[N][N] = {0};

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    heat_distribution(grid);

    MPI_Finalize();
    return 0;
}

