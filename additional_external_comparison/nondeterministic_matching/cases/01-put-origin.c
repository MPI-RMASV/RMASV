#include <mpi.h>
#include <stdio.h>

#define PROC_NUM 3
#define TAG 1

static int receive_choice(int rank) {
    int choice = -1;
    int second = -1;

    if (rank == 0) {
        MPI_Recv(&choice, 1, MPI_INT, MPI_ANY_SOURCE, TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&second, 1, MPI_INT, MPI_ANY_SOURCE, TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("first message value=%d\n", choice);
    } else {
        int value = rank - 1;
        MPI_Ssend(&value, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }
    return choice;
}

int main(int argc, char **argv) {
    int rank;
    int size;
    int *window_base;
    int origin = 1;
    MPI_Win window;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != PROC_NUM) {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Win_allocate(sizeof(int), sizeof(int), MPI_INFO_NULL,
                     MPI_COMM_WORLD, &window_base, &window);
    *window_base = 0;
    int choice = receive_choice(rank);

    MPI_Win_fence(0, window);
    if (rank == 0 && choice == 1) {
        MPI_Put(&origin, 1, MPI_INT, 1, 0, 1, MPI_INT, window);
        origin = 5;
    }
    MPI_Win_fence(0, window);

    MPI_Win_free(&window);
    MPI_Finalize();
    return 0;
}
