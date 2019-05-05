#include <stdio.h>
#include <stdbool.h>
#include <time.h>

bool actualValue(int const x, int temp[], int const lengths[], int const n){
    int tempSum = 0;
    for(int i = 0; i <= n; i++){
        tempSum = tempSum + (temp[i]*lengths[i]);
    }
    return tempSum == x;
}

bool possible_iter(int const x, int const lengths[], int const counts[]) {
    // iterative solution for 4 different lengths of a pipe
    // try every possible constelation, if one found, return true
    for(int c1 = 0; c1 <= counts[0]; c1++){
        for(int c2 = 0; c2 <= counts[1]; c2++){
            for(int c3 = 0; c3 <= counts[2]; c3++){
                for(int c4 = 0; c4 <= counts[3]; c4++){
                    if ((c1 * lengths[0] + c2 * lengths[1] + c3 * lengths[2] + c4 * lengths[3]) == x){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool possible_rec(int const x, int const lengths[], int actualLength, int const counts[], int const n, int actualPos) {
    // recursion stoppers
    if(actualLength == x){
        return true;
    }
    if (actualPos >= n){
        return false;
    }

    for (int i = 0; i <= counts[actualPos]; i++){
        actualLength += i * lengths[actualPos];
        if(possible_rec(x, lengths, actualLength, counts, n, actualPos+1)){
            return true;
        } else {
            //way was not successfull
            actualLength -= i * lengths[i];
        }
    }
    return false;
}

bool possible_bt(int const x, int const lengths[], int actualLength, int const counts[], int const n, int actualPos) {
    // recursion stoppers
    if(actualLength == x){
        return true;
    }
    if (actualPos >= n){
        return false;
    }

    for (int i = 0; i <= counts[actualPos]; i++){
        //check if length is possible
        if ((actualLength + i * lengths[actualPos]) <= x){
            actualLength += i * lengths[actualPos];
            if(possible_bt(x, lengths, actualLength, counts, n, actualPos+1)){
                return true;
            } else {
                //way was not successfull
                actualLength -= i * lengths[i];
            }
        }
    }
    return false;
}

void showStorrage(int const lengths[], int const counts[], int const n){
    printf("Used Test-storrage: \n");
    for(int i = 0; i <= n; ++i){
        printf("L: %d,  #L: %d\n", lengths[i], counts[i]);
    }
}

void showSolution(bool solution, double time, int testv){
    printf("Length of %d %s in storrage\n", testv, solution ? "available" : "not available");
    printf("time needed to solve: %.lf ms\n", time);
}

void testIterativ(){
    bool tempBool = false;
    clock_t start, end;
    double neededTime = 0.0;
    printf("Test iterativ with n = 4\n");
    int lengths[] = {5, 2, 4, 3};
    int counts[] =  {3, 6, 0, 3};
    showStorrage(lengths, counts, 3);

    printf("\n");
    neededTime = 0.0;
    start = clock();
    tempBool = possible_iter(100, lengths, counts);
    end = clock();
    neededTime = (((double) (end - start) / CLOCKS_PER_SEC) * 1000);
    showSolution(tempBool, neededTime, 100);

    printf("\n");
    neededTime = 0.0;
    start = clock();
    tempBool = possible_iter(21, lengths, counts);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    showSolution(tempBool, neededTime, 21);

    printf("\n");
    neededTime = 0.0;
    start = clock();
    tempBool = possible_iter(1, lengths, counts);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    showSolution(tempBool, neededTime, 1);
}

void testRecBT(){
    bool tempBool = false;
    clock_t start, end;
    double neededTime = 0.0;
    printf("Test recursive and backtracking\n");
    int lengths[] = {5, 2, 4, 3, 9, 11, 7};
    int counts[] =  {3, 6, 2, 3, 5, 3,  0};
    showStorrage(lengths, counts, 6);

    printf("\n");
    neededTime = 0.0;
    start = clock();
    tempBool = possible_rec(200, lengths, 0, counts, 6, 0);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    printf("rec: \n");
    showSolution(tempBool, neededTime, 200);
    neededTime = 0.0;
    start = clock();
    tempBool = possible_bt(200, lengths, 0, counts, 6, 0);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    printf("bt: \n");
    showSolution(tempBool, neededTime, 200);

    printf("\n");
    neededTime = 0.0;
    start = clock();
    tempBool = possible_rec(46, lengths, 0, counts, 6, 0);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    printf("rec: \n");
    showSolution(tempBool, neededTime, 46);
    neededTime = 0.0;
    start = clock();
    tempBool = possible_bt(46, lengths, 0, counts, 6, 0);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    printf("bt: \n");
    showSolution(tempBool, neededTime, 46);

    printf("\n");
    neededTime = 0.0;
    start = clock();
    tempBool = possible_rec(39, lengths, 0, counts, 6, 0);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    printf("rec: \n");
    showSolution(tempBool, neededTime, 39);
    neededTime = 0.0;
    start = clock();
    tempBool = possible_bt(39, lengths, 0, counts, 6, 0);
    end = clock();
    neededTime = (((double)(end - start) / CLOCKS_PER_SEC) * 1000);
    printf("bt: \n");
    showSolution(tempBool, neededTime, 39);
}

int main() {
    testIterativ();
    testRecBT();
    return 0;
}