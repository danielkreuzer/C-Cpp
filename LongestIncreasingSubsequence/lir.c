#include "lir.h"
#define MAX 100

int longest_increasing_run(int const s[], int const n){
    int lir = 0;
    int temp_lir = 1;
    for (int i = 0; i < n-1; i++){
        if(s[i] < s[i+1]){
            temp_lir++;
        } else {
            temp_lir = 1;
        }
        if (temp_lir > lir) lir = temp_lir;
    }
    return lir;
}