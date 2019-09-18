#include <stdio.h> 
 
void Print_two_d(int two_d[3][4], int rows, int cols){ 
    int i,j; 
 
    for (i=0;i<rows;i++){ 
        for (j=0;j<cols;j++){ 
            printf("A[%d][%d]=%d ", i, j, two_d[i][j]); 
        } 
        printf("\n"); 
    } 
} 
 
int main(void){ 
 
    int two_d[3][4] = {{2,3,4,5},{3,4,5,6},{4,5,6,7}}; 
 
    Print_two_d(two_d,3,4); 
 
    return 0; 
}

///Some compilers also support variable length arrays as arguments.
