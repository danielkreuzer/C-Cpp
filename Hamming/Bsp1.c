#include <stdio.h>
#include <time.h>

// check if number is in Hemming sequence
int check_number(int input){
    int temp = 0;
    if (input == 1) temp = 1;
    else if (input % 2 == 0 && check_number(input / 2)) temp = 1;
    else if (input % 3 == 0 && check_number(input / 3)) temp = 1;
    else if (input % 5 == 0 && check_number(input / 5)) temp = 1;
    return temp;
}

int main (int argc, char * argv[]) {
    int input = 0;

    if (argc == 2 && sscanf(argv[1], "%d", &input) == 1){
        clock_t start = clock();
        for(int i = 1; i <= input; i++){
            if (check_number(i)){
                printf("%d\n", i);
            }
        }
        clock_t finish = clock();
        printf("Time needed: %ld msec", ((finish - start)/CLOCKS_PER_SEC)*1000);

    } else{
        printf("Please check your input!");
    }
    
    return 0;
}