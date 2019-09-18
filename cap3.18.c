#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void read_vector( int *vc, int my_rank, int comm_sz, MPI_Datatype type)
{
    int i;
    vc = NULL;
    int n=12;

    if( my_rank == 0){
        printf("Insert 12 values to vector\n");

        for(i=0; i<n; ++i)
            scanf("%i", &vc[i]);
        for(i=1; i<comm_sz; ++i)
            MPI_Send(vc, 1, type, i, 0, MPI_COMM_WORLD);
    }
    else{
        MPI_Recv(vc, 1, type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}

void print_vector(int *vc, int my_rank, int comm_sz, MPI_Datatype type)
{
    int n=12;
    if( my_rank != 0){
        MPI_Send(vc, 1, type, 0, 0, MPI_COMM_WORLD);
    }
    else{
        int i;
        for(i=0; i<comm_sz; ++i)
            MPI_Recv(vc, 1, type, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for(i=0; i<n; ++i)
            printf("%i", vc[i]);
    }
}
int main()
{
    int my_rank, comm_sz;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Datatype type;
    MPI_Type_vector(3, 2, 4, MPI_DOUBLE, &type);
    MPI_Type_commit(&type);

    int *v = malloc(12 * sizeof(double));
    read_vector(v, my_rank, comm_sz, type);
    print_vector(v, my_rank, comm_sz, type);

    return 0;
}