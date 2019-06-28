/* C=A*B */

#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include "mpi.h"

int N = 4096;

int main(int argc, char *argv[]) {

    int mpisize, mpirank, numberof_data, rows;
    struct timeval tic, toc;

    /*MPI initialization*/
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

    numberof_data = N * N / mpisize;
    rows = N / mpisize;

    /*create matrix*/
    double **A = new double *[N];
    double **B = new double *[N];
    double **C = new double *[N];
    for (int i = 0; i < N; i++) {
        A[i] = new double[N];
        B[i] = new double[N];
        C[i] = new double[N];
        for (int j = 0; j < N; j++) {
            A[i][j] = drand48();
            B[i][j] = drand48();
            C[i][j] = 0;
        }
    }
    double *A_Small = new double[numberof_data];
    double *C_Small = new double[numberof_data];

    MPI_Barrier(MPI_COMM_WORLD);

    /* start */
    gettimeofday(&tic, NULL);

    /* send matrix A and B */
    MPI_Scatter(A, numberof_data, MPI_DOUBLE, A_Small, numberof_data, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* calculate */
    for (int i = 0; i < rows; i++) {
        C_Small[i] = 0;
        for (int j = 0; j < N; j++) {
            C_Small[i] += A_Small[j] * B[j][i];
        }
    }

    /* receive matrix C */
    MPI_Gather(C_Small, numberof_data, MPI_DOUBLE, C, numberof_data, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* stop */
    gettimeofday(&toc, NULL);

    /* time output */
    double time = toc.tv_sec - tic.tv_sec + (toc.tv_usec - tic.tv_usec) * 1e-6;
    printf("N=%d: %lf s (%lf GFlops)\n", N, time, 2. * rows * N * N / time / 1e9);

    MPI_Finalize();

}
