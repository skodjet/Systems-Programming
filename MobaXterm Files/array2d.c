#include <stdio.h>

#define ROWS 2
#define COLS 3

int row_sum[2] = {0, 0};

int* sum_rows(int arr[ROWS][COLS]) {
    
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLS; col++) {
            row_sum[row] += arr[row][col];
        }
    }
    // printf("Row 0: %d\n", row_sum[0]);
    // printf("Row 1: %d\n", row_sum[1]);

    return row_sum;

}

int main(int argc, char const *argv[]) {

    int arr[2][3] = {{1, 2, 3}, {4, 5, 6}};
    int *sums = sum_rows(arr);
    printf("Row 0: %d\n", sums[0]);

    return 0;
}