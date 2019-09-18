#include <iostream>

using namespace std;
#include <ctime> 
 

int isSorted(int* a, int n) {
    for (int i = 1; i < n; i++) {
        if (a[i] < a[i-1]) {
            return 0;
        }
    }

    return 1;
}

void Generate_list(int local_A[], int local_n, int my_rank) {
   int i;

    srandom(my_rank+1);
    for (i = 0; i < local_n; i++)
       local_A[i] = random() % 100;

} 

void Odd_even_sort(int  a[],int  n)
{
    int phase, i, temp;

    for (phase = 0; phase < n; phase++) {

        if (phase % 2 == 0) { /* Even phase */
            for (i = 1; i < n; i += 2)
                if (a[i - 1] > a[i]) {
                    temp = a[i];
                    a[i] = a[i - 1];
                    a[i - 1] = temp;
                }
        } else { /* Odd phase */
            for (i = 1; i < n - 1; i += 2)
                if (a[i] > a[i + 1]) {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
        }

        if (isSorted(a, n))
            break;
    }
}

int main()
{
    int n=80000;
    int a[n];
    Generate_list(a,n,3);
    unsigned t0, t1;
 
    t0=clock();
    Odd_even_sort(a,n);
    t1 = clock();
 
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
    


    return 0;
}
