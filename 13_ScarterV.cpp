///The following example shows how MPI _Scatterv can be used:

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
 
int main(void) { 
 
    int my_rank; 
    int comm_sz; 
    int i,j; 
 
    MPI_Init(NULL,NULL); 
 
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
 
    int data_count; 
    int send_counts_cumul; 
    int *data_array = NULL; 
    int *send_counts = NULL; 
    int *data_displ = NULL; 
 
    int *local_recv; 
    int recv_count; 
 
    if (my_rank==0) { 
        data_count = rand()%100+comm_sz; // at least one item per process 
        data_array = (int*)malloc(data_count*sizeof(int)); 
        send_counts = (int*)malloc(comm_sz*sizeof(int)); 
        data_displ = (int*)malloc(comm_sz*sizeof(int)); 
        send_counts_cumul=0; 
        *data_displ=0; 
        for (i=0;i<(comm_sz-1);i++){ 
            *(send_counts+i)=rand()%(data_count/comm_sz)+1; 
            send_counts_cumul+=*(send_counts+i); 
            *(data_displ+(i+1))=send_counts_cumul; 
        } 
        *(send_counts+(comm_sz-1))=data_count-send_counts_cumul; 
        printf("TOTAL=%d\n",data_count); 
        for (i=0;i<comm_sz;i++){ 
            printf("RANK[%d]: C=%d D=%d\n",i,*(send_counts+i), *(data_displ+i)); 
        } 
        for (i=0;i<data_count;i++) { 
            for (j=0;j<comm_sz;j++) { 
                if (*(data_displ+j)==i) { 
                    printf("\n"); 
                } 
            } 
            *(data_array+i) = rand()%100+18; 
            printf("%d ", *(data_array+i)); 
        } 
        printf("\n"); 
    } 
 
    MPI_Scatter(send_counts, 1, MPI_INT,&recv_count,1,MPI_INT,0,MPI_COMM_WORLD); 
 
    local_recv = (int *)malloc(recv_count*sizeof(int)); 
 
    MPI_Scatterv(data_array,send_counts,data_displ,MPI_INT,local_recv,recv_count,MPI_INT,0,MPI_COMM_WORLD); 
 
    for (i=0;i<recv_count;i++){ 
        printf("RANK[%d]=%d\n",my_rank, *(local_recv+i)); 
    } 
 
    if (data_array) 
        free(data_array); 
    if (send_counts) 
        free(send_counts); 
    if (data_displ) 
        free(data_displ); 
    if (local_recv) 
        free(local_recv); 
 
    MPI_Finalize(); 
 
    return 0; 
}
