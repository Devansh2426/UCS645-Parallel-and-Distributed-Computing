#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10

void odd_even_sort(int *arr, int n, int rank, int size) {
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        for (int phase = 0; phase < 2; phase++) {
            for (int i = rank; i < n - 1; i += size) {
                if ((i % 2 == phase) && arr[i] > arr[i + 1]) {
                    int temp = arr[i];
                    arr[i] = arr[i + 1];
                    arr[i + 1] = temp;
                    sorted = 0;
                }
            }
            MPI_Allreduce(MPI_IN_PLACE, &sorted, 1, MPI_INT, MPI_LAND, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char** argv) {
    int rank, size, arr[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        for (int i = 0; i < N; i++)
            arr[i] = rand() % 100;
    }

    MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);
    odd_even_sort(arr, N, rank, size);

    if (rank == 0) {
        printf("Sorted Array: ");
        for (int i = 0; i < N; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}

