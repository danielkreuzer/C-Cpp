#include <stdio.h>
#include <time.h>

//LineLength
#define LL 9

double neededTime;

int check_possibleNumber(int squares[], int row, int column, int numToCheck){
    int startRow = (row / 3) * 3;
    int startColumn = (column / 3) * 3;
    int row1 = (row + 2) % 3;
    int row2 = (row + 4) % 3;
    int column1 = (column + 2) % 3;
    int column2 = (column + 4) % 3;

    //checkout position in row and column
    for (int i = 0; i < 9; i++) {
        //check if number is valid in this position
        if (squares[i * LL + column] == numToCheck) {
            return 0;
        }
        if (squares[row * LL + i] == numToCheck) {
            return 0;
        }
    }

    //checkout the missing 4 spaces in the scope
    if (squares[(row1 + startRow) * LL + column1 + startColumn] == numToCheck)
    {
        return 0;
    }
    if (squares[(row1 + startRow) * LL + column2 + startColumn] == numToCheck)
    {
        return 0;
    }
    if (squares[(row2 + startRow) * LL + column1 + startColumn] == numToCheck)
    {
        return 0;
    }
    if (squares[(row2 + startRow) * LL + column2 + startColumn] == numToCheck)
    {
        return 0;
    }

    return 1;
}

int set_numbers(int squares[], int row, int column){
    //check 9x9 scope
    if (!(row < 9 && column < 9)) {
        return 1;
    } else {
        int actualPos = row * LL + column;
        //check if square already filled
        if (squares[actualPos])
        {
            //check next square
            if((column + 1) < 9) {
                return set_numbers(squares, row, column + 1);
            } else if((row + 1) < 9) {
                return set_numbers(squares, row + 1, 0);
            } else {
                return 1;
            }
        } else {
            //find possible number for square
            for (int i=0; i < 9; i++){
                if (check_possibleNumber(squares, row, column, i+1)){
                    //iterate through possible numbers
                    //check for every valid if it's part of a valid solution
                    squares[actualPos] = i + 1;
                    if (set_numbers(squares, row, column))
                        return 1;
                    //failed to find a valid number
                    else
                        squares[actualPos] = 0;
                }
            }
        }
        return 0;
    }
}

int sudoku_solve(int squares[]){
    neededTime = 0.0;
    clock_t start = clock();
    //start on the left top
    int ret = set_numbers(squares, 0, 0);
    clock_t end = clock();
    neededTime = (((double) (end - start) / CLOCKS_PER_SEC) * 1000);
    return ret;
}

void print_sudoku(int squares[]){
    printf("|-----|-----|-----|\n");
    if (sudoku_solve(squares))
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                printf("|%d", squares[i * LL + j]);
            }

            printf("|\n");

            if ((i + 1) % 3 == 0)
            {
                printf("|-----|-----|-----|\n");
            }
        }
        printf("time needed to solve: %.lf ms\n", neededTime);
    }
    else
    {
        printf("no solution found \n");
        printf("time needed to solve: %.lf ms\n", neededTime);
    }
}

int main()
{
    
    int testPuzzle1[] =     {   0,7,5, 0,9,0, 0,0,6,
                                0,2,3, 0,8,0, 0,4,0,
                                8,0,0, 0,0,3, 0,0,1,
                                5,0,0, 7,0,2, 0,0,0,
                                0,4,0, 8,0,6, 0,2,0,
                                0,0,0, 9,0,1, 0,0,3,
                                9,0,0, 4,0,0, 0,0,7,
                                0,6,0, 0,7,0, 5,8,0,
                                7,0,0, 0,1,0, 3,9,0};

    int testPuzzle2[] =     {   0,7,1, 0,9,0, 8,0,0,
                                0,0,0, 3,0,6, 0,0,0,
                                4,9,0, 0,0,0, 7,0,5,
                                0,1,0, 9,0,0, 0,0,0,
                                9,0,2, 0,0,0, 6,0,3,
                                0,0,0, 0,0,8, 0,2,0,
                                8,0,5, 0,0,0, 0,7,6,
                                0,0,0, 6,0,7, 0,0,0,
                                0,0,7, 0,4,0, 3,5,0};

    int testPuzzle3[] =     {   0,7,1, 0,9,0, 8,0,0,
                                0,0,0, 9,0,0, 0,0,0,
                                4,9,0, 0,0,0, 7,0,5,
                                0,1,0, 9,0,0, 0,0,0,
                                9,0,2, 9,0,9, 0,0,0,
                                0,0,0, 0,0,8, 0,2,0,
                                8,0,5, 0,0,0, 0,7,6,
                                0,0,0, 6,0,7, 0,0,0,
                                0,0,7, 0,4,0, 3,5,0};

    int testPuzzleL[] =     {   0,0,0, 0,0,0, 0,0,0,
                                0,0,0, 0,0,3, 0,8,5,
                                0,0,1, 0,2,0, 0,0,0,
                                0,0,0, 5,0,7, 0,0,0,
                                0,0,4, 0,0,0, 1,0,0,
                                0,9,0, 0,0,0, 0,0,0,
                                5,0,0, 0,0,0, 0,7,3,
                                0,0,2, 0,1,0, 0,0,0,
                                0,0,0, 0,4,0, 0,0,9};

    printf("Test 1.\n");
    print_sudoku(testPuzzle1);

    printf("\nTest 2.\n");
    print_sudoku(testPuzzle2);

    printf("\nTest 3.\n");
    print_sudoku(testPuzzle3);

    printf("\nSudoku with long runtime\n");
    print_sudoku(testPuzzleL);

    return 0;
}