#include <stdio.h>

void printGrid(int grid[100][100], int N) { /* Prints the given grid */
    int row, col;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++)
            printf("%d ", grid[row][col]);
        printf("\n");
    }
}

int calculate(int grid[100][100], int n, int row, int col){
    /* Calculation with Levenshtein algorithm */
    if (row == n && col == n)
        return grid[n-1][n-1];
    else if (col == n)
        return calculate(grid, n, row+1, 0);
    else {
        if (row == 0) {
            if (!col)
                return calculate(grid, n, row, col+1);
            else {
                grid[0][col] += grid[0][col-1];
                return calculate(grid, n, row, col+1);
            }
        }
        else {
            if (col == 0)
                grid[row][col] += grid[row-1][col];
            else {
                if (grid[row-1][col] > grid[row][col-1])
                    grid[row][col] += grid[row-1][col];
                else
                    grid[row][col] += grid[row][col-1];
            }
            return calculate(grid, n, row, col+1);
        }
    }
}

void find(int grid[100][100], int n, int row, int col, char path[142], int i){
    if (row == 0 && col == 0)
        return;
    else if (col == -1)
        find(grid, n, row-1, n-1, path, i);
    else {
        if (row == 0) {
            path[i] = 'E';
            find(grid, n, row, col-1, path, i+1);
        }
        else if (col == 0) {
            path[i] = 'S';
            find(grid, n, row-1, col, path, i+1);
        }
        else {
            if (grid[row-1][col] > grid[row][col-1]) {
                path[i] = 'S';
                find(grid, n, row-1, col, path, i+1);
            }
            else {
                path[i] = 'E';
                find(grid, n, row, col-1, path, i+1);
            }
        }
    }
}

int main(){
    int n, grid[100][100], i, j, energy=0;
    char path[142], *end;

    scanf("%d ", &n);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            scanf("%d ", &grid[i][j]);

    energy = calculate(grid, n, 0, 0) * 40;
    find (grid, n, n-1, n-1, path, 0);

    for (i = 0; i < 142; i++) { /* mark the end of the reverse path */
        if (!path[i]){
            end = &path[i-1];
            break;
        }
    }

    while (*end == 'S' || *end == 'E') /* print the reverse path in the correct order */
        printf("%c ", *end--);

    printf(", final energy: %d\n", energy);

    return 0;
}
