#include "lis.h"
#include <stdio.h>
#define MAX 100

int longest_increasing_subsequence(int const s[], int const n) {
    int p[MAX] = {-1};
    for (int i = 1; i <= n-1; i++){
        int temp_pos = -1;
        for(int j=i-1; j >= 0; j--){
            if(s[i] > s[j]){
                if (temp_pos != -1 && s[temp_pos]>s[j]){
                    temp_pos = j;
                } else {
                    temp_pos = j;
                }
                if(temp_pos != -1 && p[temp_pos] != -1) break;
            } 
        }
        p[i]=temp_pos;
        //for(int j = i-1; j >= 0; j--){}
    }

    int l[MAX] = {0};
    l[0] = 1; //needs to be 1
    int temp_lis = 0;
    for (int i = 1; i <= n-1; i++){
        if(p[i] != -1){
            int temp_jmp = 1;
            int j = p[i];
            while (j != -1){
                temp_jmp++;
                j = p[j];
            }
            l[i] = temp_jmp;
            if(temp_jmp > temp_lis) temp_lis = temp_jmp;
        } else {
            l[i] = 1;
        }
    }

    //print arrays
    printf("\np:\n");
    for (int i = 0; i <= n-1; i++){
        printf("%d: %d\n", i, p[i]);
    }
    printf("\nl:\n");
    for (int i = 0; i <= n-1; i++){
        printf("%d: %d\n", i, l[i]);
    }
    return temp_lis;
}