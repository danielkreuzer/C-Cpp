#include <stdio.h>
#include <limits.h>

#define MAX 100

//merge sort start
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
    
    SplitMerge(a, begin, middle, b);  // sort the left  run
    SplitMerge(a, middle, end, b);  // sort the right 
    
    // merge the resulting runs from array B[] into A[]
    Merge(b, begin, middle, end, a);
}

void merge_sort (int a[], int n) {
    int b[MAX] = {0};
    CopyArray(a, 0, n, b);     
    SplitMerge(b, 0, n, a);
}
//merge sort end

//quicksort Start
void swap(int a[], int pos_a, int b[], int pos_b){
    int temp = a[pos_a];
    a[pos_a] = b[pos_b];
    b[pos_b] = temp;
}
 
 
int partition(int a[], int left, int right){
 
    int pivot = a[right], x = left;
 
    for (int i = left; i < right; i++){
        if (a[i] >= pivot){
            swap(a, x, a, i);
            x++;
        }
    }
 
    swap(a, x, a, right);
    return x;
}
 
 
int mod_quicksort(int a[], int left, int right, int k){
 
    //p is position of pivot in the partitioned array
    int p = partition(a, left, right);
 
    //k = pivot
    if (p == k-1){
        return a[p];
    }
    //k < pivot
    else if (k-1 < p){
        return mod_quicksort(a, left, p - 1, k);
    }
    //k > pivot
    else{
        return mod_quicksort(a, p + 1, right, k);
    }
}
//quicksort end

int secound_largest(int a[], int n){
    int largest = INT_MIN;
    int sec_largest = INT_MIN;
    for (int i = 0; i <= n; i++){
        if (a[i] > largest) largest = a[i];
    }
    for (int i = 0; i <= n; i++){
        if (a[i] < largest && a[i] > sec_largest) sec_largest = a[i];
    }
    return sec_largest;
}

int ith_largest_1 (int a[], int n, int i){
    return a[n-i];
}

int ith_largest_2(int a[], int n, int i){
    int left = 0; 
    int right = n-1; 
 
    return mod_quicksort(a, left, right, i);
}

int main(int argc, char * argv[]){
    //check input
    //min needed params = 3 (2 for secound largest + 1 for i largest)
    if (argc < 4 || argc > MAX + 2){
        printf("Wrong input \n");
        return -1;
    }

    int i_select = 0;
    int a[MAX] = {0};
    int b[MAX] = {0};
    int n = argc - 2;
    printf("n: %d\n", n);
    //fill array
    printf("Input: \n");
    if (sscanf(argv[n+1], "%d", &i_select) != 1){
        printf("Wrong i-select parameter");
        return -2;
    }
    for (int i = 1; i <= n; i++) {
        int input = 0;
        if (sscanf(argv[i], "%d", &input) != 1){
            printf("Wrong parameter on position %d\n", i);
            return -1;
        } else {
            a[i-1] = input;
            printf("%d ", a[i-1]);
        }
    }
    
    //(a) secound-largest
    printf("\n");
    printf("Secound largest element: %d\n", secound_largest(a, n));

    // (b) ith-largest with Merge-Sort
    //copy array for merge Sort
    CopyArray(a, 0, n, b);
    merge_sort(b, n);
    //show sorted array
    printf("Merge-Sorted: \n");
    for (int i = 0; i <= n-1; i++){
        printf("%d ", b[i]);
    }
    printf("\n");
    printf("%d-largest element: %d\n", i_select, ith_largest_1(b, n, i_select));

    //(c) ith-largest with modified quicksort
    printf("%d-largest element with mod_quicksort: %d", i_select, ith_largest_2(a, n,  i_select));
}