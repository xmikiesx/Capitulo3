 
#include <stdio.h> 
#include <stdlib.h> 
 
#define SIZE 16 
 
int main(void) { 
 
    int a[SIZE]; 
    int sum[SIZE]; 
    int i; 
 
    for (i=0; i<SIZE; i++){ 
        a[i]=rand()%50; 
    } 
    sum[0]=a[0]; 
    for (i=1; i<SIZE; i++) { 
        sum[i]=sum[i-1]+a[i]; 
    } 
 
    for (i=0;i<SIZE;i++){ 
        printf("A[%2d]=%9d SUM[%2d]=%9d\n", i, a[i], i, sum[i]); 
    } 
 
 
    return 0; 
} 
