#include <mpi.h>
#include <stdio.h>

#define PROC_NUM 3

#ifdef RMASV_SYMBOLIC
extern void klee_assume(unsigned long condition);
extern void klee_make_symbolic(void *address, unsigned long size, const char *name);
#endif

static int get_input_choice(int argc, char **argv, int rank) {
    int choice = 0;
    if (rank == 0) {
#ifdef RMASV_SYMBOLIC
        klee_make_symbolic(&choice, sizeof(choice), "choice");
        klee_assume(choice == 0 || choice == 1);
#else
        choice = argc > 1 && argv[1][0] == '1';
#endif
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
    int choice = get_input_choice(argc, argv, rank);

    MPI_Win_fence(0, window);
    if (rank == 0) {
        if (choice) {
            MPI_Put(&origin, 1, MPI_INT, 1, 0, 1, MPI_INT, window);
        }
    } else if (rank == 1) {
        *window_base = 5;
    }
    MPI_Win_fence(0, window);

    MPI_Win_free(&window);
    MPI_Finalize();
    return 0;
}
