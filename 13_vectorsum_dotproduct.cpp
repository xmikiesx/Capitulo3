#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
 
int main(void){ 
 
    int comm_sz; 
    int my_rank; 
    int i,j; 
 
    int data_count; 
    int send_counts_cumul; 
    int *send_data_array1 = NULL; 
    int *send_data_array2 = NULL; 
    int *send_counts = NULL; 
    int *data_displ = NULL; 
    int dot_product_local = 0; 
    int dot_product; 
 
    MPI_Init(NULL,NULL); 
 
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); 
 
    int *local_recv1; 
    int *local_recv2; 
 
    int recv_count; 
 
    if (my_rank==0) { 
        data_count = rand()%50+comm_sz; // at least one item per process 
        send_data_array1 = (int*)malloc(data_count*sizeof(int)); 
        send_data_array2 = (int*)malloc(data_count*sizeof(int)); 
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
            printf("RANK[%2d]: C=%3d D=%3d\n",i,*(send_counts+i), *(data_displ+i)); 
        } 
        for (i=0;i<data_count;i++) { 
            for (j=0;j<comm_sz;j++) { 
                if (*(data_displ+j)==i) { 
                    printf("\n"); 
                } 
            } 
            *(send_data_array1+i) = rand()%100+18; 
            *(send_data_array2+i) = rand()%100+109; 
            printf("[0]%3d [1]%3d ", *(send_data_array1+i), *(send_data_array2+i)); 
        } 
        printf("\n"); 
    } 
 
    MPI_Barrier(MPI_COMM_WORLD); 
 
    MPI_Scatter(send_counts, 1, MPI_INT,&recv_count,1,MPI_INT,0,MPI_COMM_WORLD); 
 
    local_recv1 = (int *)malloc(recv_count*sizeof(int)); 
    local_recv2 = (int *)malloc(recv_count*sizeof(int)); 
 
    MPI_Scatterv(send_data_array1,send_counts,data_displ,MPI_INT,local_recv1,recv_count,MPI_INT,0,MPI_COMM_WORLD); 
    MPI_Scatterv(send_data_array2,send_counts,data_displ,MPI_INT,local_recv2,recv_count,MPI_INT,0,MPI_COMM_WORLD); 
 
    for (i=0;i<recv_count;i++){ 
        //printf("RANK[%2d] [0]%3d [1]%3d\n",my_rank, *(local_recv1+i), *(local_recv2+i)); 
        dot_product_local+=(*(local_recv1+i))*(*(local_recv2+i)); 
        // store the local vector sums in local_recv1 
        *(local_recv1+i)+=*(local_recv2+i); 
    } 
    printf("RANK[%2d] LOCAL_DOT_PRODUCT=%d\n",my_rank,dot_product_local); 
    // send the vector sum using MPI_Gatherv to send_data_array1 
    MPI_Gatherv(local_recv1,recv_count,MPI_INT,send_data_array1,send_counts,data_displ,MPI_INT,0,MPI_COMM_WORLD); 
    // compute the dot product using MPI_Reduce 
    MPI_Reduce(&dot_product_local,&dot_product,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD); 
 
    if (my_rank==0){ 
        printf("DOT_PRODUCT=%d\n",dot_product); 
        for (i=0;i<data_count;i++){ 
            printf("SUM[%2d]=%d\n",i,*(send_data_array1+i)); 
        } 
    } 
 
    if (send_data_array1) 
        free(send_data_array1); 
    if (send_counts) 
        free(send_counts); 
    if (data_displ) 
        free(data_displ); 
    if (local_recv1) 
        free(local_recv1); 
    if (local_recv2) 
        free(local_recv2); 
 
    MPI_Finalize(); 
 
    return 0; 
}
