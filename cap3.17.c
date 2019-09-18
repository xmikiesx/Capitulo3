#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void read_vector(double local_vector[], int local_n, int n, int my_rank, MPI_Datatype type)
{
    int i;
    double *vector = NULL;

    if( my_rank == 0){
        vector = malloc(n*sizeof(double));
        printf("Insert values to vector\n");

        for(i=0; i<n; ++i)
            scanf("%d", &vector);
    }
    MPI_Scatter(vector, 1, type, local_vector, 1, type, 0, MPI_COMM_WORLD);
    free(vector);
}

void print_vector(double local_vector[], int local_n, int n, int my_rank, MPI_Datatype type)
{
    double *vector = NULL;

    if( my_rank == 0){
        vector = malloc(n*sizeof(double));
        MPI_Gather(local_vector, 1, type, vector, 1, type, 0, MPI_COMM_WORLD);

        for(int i=0; i<n; ++i)
            printf("%f", vector[i]);
        free(vector);
    }
    else{
        MPI_Gather(local_vector, 1, type, vector, 1, type, 0, MPI_COMM_WORLD);
    }
}

int main()
{
    MPI_Datatype type;
    int my_rank;    
    int local_n = 4;
    int n = 12;
    MPI_Init(NULL, NULL);
    
    MPI_Type_contiguous(local_n, MPI_CHAR, &type);
    MPI_Time_commit(&type);

    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    double *local_vector = malloc(local_n * sizeof(double));
    read_vector(local_vector, local_n, n, my_rank, type);
    print_vector(local_vector, local_n, n, my_rank, type);

    MPI_Type_free(&type);
    MPI_Finalize();
}
