#include <stdio.h> 
#include <mpi.h> 
 
int main(void) { 
 
    int num_procs, local_proc, proc_iter; 
    char dummy=0; 
 
    MPI_Init(NULL,NULL); 
    MPI_Comm_size(MPI_COMM_WORLD,&num_procs); 
    MPI_Comm_rank(MPI_COMM_WORLD,&local_proc); 
 
    if (local_proc!=0){ 
        MPI_Send(&dummy,1,MPI_CHAR,0,0,MPI_COMM_WORLD); 
    } else { 
        printf("NUM_PROCS=%d, CURR_PROC=%d\n", num_procs, local_proc); 
        for (proc_iter=1;proc_iter<num_procs;proc_iter++){ 
            MPI_Recv(&dummy,1,MPI_CHAR,proc_iter,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); 
            printf("NUM_PROCS=%d, CURR_PROC=%d\n", num_procs, proc_iter); 
        } 
    } 
 
    MPI_Finalize(); 
 
    return 0; 
}
