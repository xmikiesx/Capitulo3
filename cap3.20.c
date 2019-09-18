#include <stdio.h>
#include <mpi.h>
//mpicc -g -o mpicap3 cap3.20.c
//mpiexec -n 2 ./mpicap3
 
void Build_mpi_type(double* a_p, double* b_p, int* n_p,MPI_Datatype* input_mpi_t_p);
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);
void Get_input_pack(int my_rank, int comm_sz, double* a_p, double* b_p,int* n_p);
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);    
double f(double x); 

int main(void) 
{
   int my_rank, comm_sz, n, local_n;   
   double a, b, h, local_a, local_b;
   double local_int, total_int;

   MPI_Init(NULL, NULL);

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   Get_input_pack(my_rank, comm_sz, &a, &b, &n);

   h = (b-a)/n;          
   //number trapezoides
   local_n = n/comm_sz;  

   //Length of each process' interval of integration = local_n*h. 
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   /* Add up the integrals calculated by each process */
   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   if (my_rank == 0) {
      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %.15e\n",
          a, b, total_int);
   }

   MPI_Finalize();
   return 0;
} 

//FUNCTIONS
// Build a derived datatype for distributing the input data
void Build_mpi_type(double* a_p, double* b_p,int* n_p,  MPI_Datatype*  input_mpi_t_p) {

   int array_of_blocklengths[3] = {1, 1, 1};
   MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
   MPI_Aint a_addr, b_addr, n_addr;
   MPI_Aint array_of_displacements[3] = {0};

   MPI_Get_address(a_p, &a_addr);
   MPI_Get_address(b_p, &b_addr);
   MPI_Get_address(n_p, &n_addr);
   array_of_displacements[1] = b_addr-a_addr; 
   array_of_displacements[2] = n_addr-a_addr; 
   MPI_Type_create_struct(3, array_of_blocklengths, 
         array_of_displacements, array_of_types,
         input_mpi_t_p);
   MPI_Type_commit(input_mpi_t_p);
}

void Get_input(int my_rank,int comm_sz, double*  a_p, double*  b_p, int* n_p) {
   MPI_Datatype input_mpi_t;

   Build_mpi_type(a_p, b_p, n_p, &input_mpi_t);

   if (my_rank == 0) {
      printf("Enter a, b, and n\n");
      scanf("%lf %lf %d", a_p, b_p, n_p);
   } 
   MPI_Bcast(a_p, 1, input_mpi_t, 0, MPI_COMM_WORLD);

   MPI_Type_free(&input_mpi_t);
} 

void Get_input_pack(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
    char pack_buf[1000];
    if (my_rank == 0) {
        int position = 0;
        printf("Enter a, b, and n\n");
        scanf("%lf %lf %d", a_p, b_p, n_p);
        MPI_Pack(a_p, 1, MPI_DOUBLE, pack_buf, 1000, &position, MPI_COMM_WORLD);
        MPI_Pack(b_p, 1, MPI_DOUBLE, pack_buf, 1000, &position, MPI_COMM_WORLD);
        MPI_Pack(n_p, 1, MPI_INT, pack_buf, 1000, &position, MPI_COMM_WORLD);
    }
    int positionUnpack = 0;
    MPI_Bcast(pack_buf, 1000, MPI_PACKED, 0, MPI_COMM_WORLD);
    MPI_Unpack(pack_buf, 1000, &positionUnpack, a_p, 1, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(pack_buf, 1000, &positionUnpack, b_p, 1, MPI_DOUBLE, MPI_COMM_WORLD);
    MPI_Unpack(pack_buf, 1000, &positionUnpack, n_p, 1, MPI_INT, MPI_COMM_WORLD);

}

double Trap(double left_endpt, double right_endpt, int    trap_count, double base_len) {
   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
}

double f(double x) {
   return x*x;
} 
