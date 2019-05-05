#include <stdlib.h>
#include <stdio.h>
#include "lis.h"
#include "lir.h"

#define MAX 100

int main(){
    int const s[MAX] = {9, 5, 2, 8, 7, 3, 1, 6, 4};
    int const n = 9;
    int const s2[MAX] = {1,2,3,4,5,6,7,8,9,10};
    int const n2 = 10;
    int const s3[MAX] = {-9, -5, -5, 2, 8, 7, 6, 1, 9};
    int const n3 = 9;
    int const s4[MAX] = {1};
    int const n4 = 1;

    printf("Test1 LIR: %d", longest_increasing_run(s, n));
    printf("Test1 LIS: %d\n", longest_increasing_subsequence(s, n));
    printf("\nTest2 LIR: %d", longest_increasing_run(s2, n2));
    printf("Test2 LIS: %d\n", longest_increasing_subsequence(s2, n2));
    printf("\nTest3 LIR: %d", longest_increasing_run(s3, n3));
    printf("Test3 LIS: %d\n", longest_increasing_subsequence(s3, n3));
    printf("\nTest4 LIR: %d", longest_increasing_run(s4, n4));
    printf("Test4 LIS: %d\n", longest_increasing_subsequence(s4, n4));
}