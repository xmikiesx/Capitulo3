 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <math.h> 
#include <mpi.h> 
 
int main(void){ 
    int my_rank; 
    int comm_sz; 
 
    int *local_array; 
    int *local_buffer_array; 
    int *prefix_sum; 
 
    int local_val; 
    int i,j,d; 
 
    MPI_Init(NULL,NULL); 
 
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz); 
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank); 
 
    if ( (1<<(int)round(log10(comm_sz)/log10(2)))!=comm_sz ) { 
        if (my_rank==0) 
            printf("Number of processors (%d) is not a power of 2!\n", comm_sz); 
        MPI_Finalize(); 
        return 0; 
    } 
 
    local_array = (int*)malloc(comm_sz*sizeof(int)); 
    local_buffer_array = (int*)malloc(comm_sz*sizeof(int)); 
    prefix_sum = (int*)malloc(comm_sz*sizeof(int)); 
 
    if (my_rank==0) { 
        for (i=0;i<comm_sz;i++){ 
            *(local_array+i)=(rand()%50+18); 
            printf("D[%d]=%d\n",i,*(local_array+i)); 
        } 
    } 
    MPI_Bcast(local_array,comm_sz,MPI_INT,0,MPI_COMM_WORLD); 
 
    // tree up 
    for (d=1; d<=round(log10(comm_sz)/log10(2));d++){ 
        if ( ((my_rank+1)%(1<<d))==0 ) { 
            *(local_array+my_rank)=*(local_array+(my_rank-(1<<(d-1))))+*(local_array+my_rank); 
        } 
        for (i=0;i<comm_sz;i++) { 
            // keep updating our local value since lower ranks are going to overwrite 
            // the value we computed above 
            *(local_buffer_array+my_rank)=*(local_array+my_rank); 
            // each rank will have to call MPI_Bcast 
            MPI_Bcast(local_buffer_array, comm_sz, MPI_INT, i, MPI_COMM_WORLD); 
        } 
        // copy for the next iteration of d 
        memcpy(local_array,local_buffer_array,comm_sz*sizeof(int)); 
    } 
 
    // save last prefix sum 
    *(prefix_sum+comm_sz-1)=*(local_array+comm_sz-1); 
 
    //if (my_rank==0) { 
    //    for (i=0;i<comm_sz;i++) 
    //        printf("RANK[%d]UPBCAST[%d]=%d\n",my_rank,i,*(local_array+i)); 
    //} 
 
    // tree down 
    // clear root node 
    *(local_array+comm_sz-1)=0; 
 
    for (d=round(log10(comm_sz)/log10(2));d>0;d--){ 
 
        // compute local changes 
        if ( ((my_rank+1)%(1<<d))==0 ) { 
            // save right node val 
            local_val = *(local_array+my_rank); 
 
            // update right node as sum top_right+left 
            *(local_array+my_rank)=*(local_array+(my_rank-(1<<(d-1))))+*(local_array+my_rank); 
 
            // update left node 
            *(local_array+(my_rank-(1<<(d-1)))) = local_val; 
        } 
 
        // then update everyone below 
        for (i=(1<<d);i<=comm_sz;i=i+(1<<d)) { 
            // keep updating our local value since lower ranks are going to overwrite 
            // the value we computed above 
            *(local_buffer_array+my_rank)=*(local_array+my_rank); 
            *(local_buffer_array+(my_rank-(1<<(d-1)))) = *(local_array+(my_rank-(1<<(d-1)))); 
            // each rank will have to call MPI_Bcast 
            MPI_Bcast(local_buffer_array, comm_sz, MPI_INT, i-1, MPI_COMM_WORLD); 
        } 
 
        // sync other’s updates 
        memcpy(local_array,local_buffer_array,comm_sz*sizeof(int)); 
    } 
 
    //if (my_rank==0) { 
    //    for (i=0;i<comm_sz;i++) 
    //        printf("RANK[%d]DOWNBCAST[%d]=%d\n",my_rank,i,*(local_array+i)); 
    //} 
 
    if (my_rank==0) { 
        memcpy(prefix_sum,local_array+1,(comm_sz-1)*sizeof(int)); 
        for (i=0;i<comm_sz;i++) 
            printf("PREFIXSUM[%d]=%d\n",i,*(prefix_sum+i)); 
    } 
 
    free(local_array); 
    free(local_buffer_array); 
    free(prefix_sum); 
 
    MPI_Finalize(); 
 
    return 0; 
}
