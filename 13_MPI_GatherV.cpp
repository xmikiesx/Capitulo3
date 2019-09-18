#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
 
int main(void){ 
    int comm_sz; 
    int my_rank; 
 
    int local_recv_count; 
    int *local_recv; 
 
    int i; 
 
    int *recv_counts=NULL; 
    int recv_counts_total=0; 
    int *recv_data=NULL; 
    int *recv_counts_cumul=NULL; 
 
    MPI_Init(NULL,NULL); 
 
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); 
 
    srand(my_rank+rand()); 
    local_recv_count = rand()%20+1; // some random number of elements to gather from each node 
    local_recv = (int*)malloc(local_recv_count*sizeof(int)); 
    printf("RANK=%d, C=%d D=", my_rank, local_recv_count); 
    for(i=0;i<local_recv_count;i++) { 
        *(local_recv+i)=rand()%900+100; 
        printf("%d ", *(local_recv+i)); 
    } 
    printf("\n"); 
 
    if (my_rank==0) 
        recv_counts=(int*)malloc(comm_sz*sizeof(int)); 
 
    MPI_Gather(&local_recv_count,1,MPI_INT,recv_counts,1,MPI_INT,0,MPI_COMM_WORLD); 
 
    if (my_rank==0) { 
        recv_counts_cumul=(int*)malloc(comm_sz*sizeof(int)); 
        for (i=0;i<comm_sz;i++){ 
            *(recv_counts_cumul+i)=recv_counts_total; 
            recv_counts_total+=*(recv_counts+i); 
            printf("C=%d D=%d\n",*(recv_counts+i),*(recv_counts_cumul+i)); 
        } 
        recv_data=(int*)malloc(recv_counts_total*sizeof(int)); 
    } 
    MPI_Gatherv(local_recv,local_recv_count,MPI_INT,recv_data,recv_counts,recv_counts_cumul,MPI_INT,0,MPI_COMM_WORLD); 
 
    if (my_rank==0){ 
        for (i=0;i<recv_counts_total;i++) 
            printf("%d ", *(recv_data+i)); 
        printf("\n"); 
    } 
 
 
    if (local_recv) 
        free(local_recv); 
    if (recv_counts) 
        free(recv_counts); 
    if (recv_data) 
        free(recv_data); 
    if (recv_counts_cumul) 
        free(recv_counts_cumul); 
 
    MPI_Finalize(); 
 
    return 0; 
}
