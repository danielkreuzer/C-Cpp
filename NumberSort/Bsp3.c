#include <stdio.h>
#define MAX 100

// copy a in b
void CopyArray(int a[], int begin, int end, int b[])
{
    for(int i = begin; i < end; i++)
        b[i] = a[i];
}


void Merge(int a[], int begin, int middle, int end, int b[])
{
    int i = begin;
    int j = middle;
 
    for (int k = begin; k < end; k++) {
        
        if (i < middle && (j >= end || a[i] <= a[j])) {
            b[k] = a[i];
            i++;
        } else {
            b[k] = a[j];
            j++;
        }
    }
}

void SplitMerge(int b[], int begin, int end, int a[])
{
    if(end - begin < 2) return;                                 
    int middle = (end + begin) / 2;             
    
    SplitMerge(a, begin, middle, b);  // sort left
    SplitMerge(a, middle, end, b);  // sort right 
    
    // merge  b[] into a[]
    Merge(b, begin, middle, end, a);
}

void merge_sort (int a[], int n) {
    int b[MAX] = {0};
    CopyArray(a, 0, n, b);     
    SplitMerge(b, 0, n, a);
}

int main (int argc, char * argv[]) {
    if (argc > MAX + 1 || argc < 2){
        printf("Wrong input\n");
    } else {
        int n = 0;
        int a[MAX] = {0};

        // code to read a maximum of MAX values from argv to a and
        // to set n to the actual number of values in a
        // code to display the unsorted array a

        n = argc - 1;
        printf("Unsorted: \n");
        for (int i = 1; i <= n; i++) {
            int input = 0;
            if (sscanf(argv[i], "%d", &input) != 1){
                printf("Wrong parameter on position %d\n", i+1);
                return -1;
            } else {
                a[i-1] = input;
                printf("%d ", a[i-1]);
            }
        }
        
        merge_sort (a, n);

        // code to display the sorted array a

        printf("\n");
        printf("Sorted: \n");
        for (int i = 0; i <= n-1; i++){
            printf("%d ", a[i]);
        }
    }
    return 0;
}