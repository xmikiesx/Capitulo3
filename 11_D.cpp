#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h> 
 
int main(void){ 
    int my_rank; 
    int comm_sz; 
 
    int *local_array=NULL; 
    int local_value; 
    int local_prefix_sum; 
    int *prefix_sum=NULL; 
 
    int local_val; 
    int i,j,d; 
 
    MPI_Init(NULL,NULL); 
 
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); 
 
    if (my_rank==0) { 
        prefix_sum = (int*)malloc(comm_sz*sizeof(int)); 
        local_array = (int*)malloc(comm_sz*sizeof(int)); 
        // generate random data on node 0 
        for (i=0;i<comm_sz;i++){ 
            *(local_array+i)=(rand()%50+18); 
            printf("D[%d]=%d\n",i,*(local_array+i)); 
        } 
        // distribute data to other nodes 
        MPI_Scatter(local_array,1,MPI_INT,&local_value,1,MPI_INT,0,MPI_COMM_WORLD); 
    } else { 
        // distribute data to other nodes 
        MPI_Scatter(local_array,1,MPI_INT,&local_value,1,MPI_INT,0,MPI_COMM_WORLD); 
    } 
 
    // compute prefix sum in each node 
    MPI_Scan(&local_value, &local_prefix_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD); 
 
    // gather prefix sum from each node to node zero 
    MPI_Gather(&local_prefix_sum,1, MPI_INT, prefix_sum, 1, MPI_INT, 0, MPI_COMM_WORLD); 
 
    if (my_rank==0) 
        for (i=0;i<comm_sz;i++) 
            printf("PREFIXSUM[%d]=%d\n",i,*(prefix_sum+i)); 
 
    if (local_array) 
        free(local_array); 
    if (prefix_sum) 
        free(prefix_sum); 
 
    MPI_Finalize(); 
 
    return 0; 
}
