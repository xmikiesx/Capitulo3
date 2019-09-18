#include <stdio.h> 
#include <stdlib.h> 
 
int main(int argc, char** argv){ 
 
    int n,i,j,psum; 
    int *arrayval; 
 
    printf("Ingrese el numero de elementos del array: "); 
    scanf("%d", &n); 
 
    arrayval = (int *)malloc(n*sizeof(int)); 
 
    if (arrayval==NULL) 
        return 1; 
 
    for (i=0;i<n;i++){ 
        psum=rand()%50; 
        *(arrayval+i)=psum; 
        printf("x[%2d]=%3d, x[%2d]",i, psum, i); 
        for (j=(i-1);j>=0;j--){ 
            printf("+x[%2d]",j); 
            psum+=*(arrayval+j); 
        } 
        printf("=%d\n",psum); 
    } 
 
    if (arrayval) 
        free(arrayval); 
 
    return 0; 
}
