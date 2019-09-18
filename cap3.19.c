#include <stdio.h>
#include <mpi.h>

//mpicc -g -o mpicap3 19.c
//mpiexec -n 2 ./mpicap3

void readMatrix(double* matrix, int tam) {
    printf("Digite a matrix nxn:\n");
    int i;
    for (i = 0; i < tam; i++) {
        int j;
        for (j = 0; j < tam; j++) {

            scanf("%lf", &matrix[i*tam+j]);
        }
    }
}

void printMatrix(double* matrix, int tam) {
    printf("matrix:\n");
    int i;
    for (i = 0; i < tam; i++) {
        int j;
        for (j = 0; j < tam; j++) {
            printf("%f ", matrix[i*tam + j]);
        }
        printf("\n");
    }
}

void constructTypeIndexed(double* matrix, int n, MPI_Datatype* newTypeBuilt) {
    int array_tamTwoBloq[n];
    int array_distances[n];
    array_distances[0] = 0;
    int i;
    int j;
    for (i = 0, j = n; i < n; i++, j--) {
        //Array sizes decrease -1 per row (upper triangle)
        array_tamTwoBloq[i] = j;
    }
    for (i = 1; i < n;  i++) {
        //His offset is the offset of the Current element - that of the first Element.
        array_distances[i] = i*n + i;
    }
    MPI_Type_indexed(n, array_tamTwoBloq, array_distances, MPI_DOUBLE, newTypeBuilt);
    MPI_Type_commit(newTypeBuilt);
}

void receiveN(int * n, int my_rank, MPI_Comm mpi_comm) {
    if (my_rank == 0) {
        printf("Digite a tam de n:\n");
        scanf("%d", n);
        MPI_Send(n, 1, MPI_INT, 1, 0, mpi_comm);
    } else {
        MPI_Recv(n, 1, MPI_INT, 0, 0, mpi_comm, MPI_STATUS_IGNORE);
    }
}

int main()
{
    int my_rank, comm_sz;
    int n;
    MPI_Comm mpi_comm = MPI_COMM_WORLD;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(mpi_comm, &comm_sz);
    MPI_Comm_rank(mpi_comm, &my_rank);

    receiveN(&n, my_rank, mpi_comm);
    double matrix[n*n];
    MPI_Datatype newType;
    constructTypeIndexed(matrix, n, &newType);
    if (my_rank == 0) {
        readMatrix(matrix, n);
        MPI_Send(matrix, 1, newType, 1, 0, mpi_comm);
    } else {
        MPI_Recv(matrix, 1, newType, 0, 0, mpi_comm, MPI_STATUS_IGNORE);
        printMatrix(matrix, n);
    }
    MPI_Type_free(&newType);
    MPI_Finalize();
    return 0;
}