#include <stdio.h>

void printGrid(int grid[100][100], int N) { /* Prints the given grid */
    int row, col;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            printf("%d ", grid[row][col]);
        }
        printf("\n");
    }
}


void mergeHelper(int grid[100][100], int gridSize, int *row, int *col, int *recursionDepth,
    int *mergeCondition, int coordsToMerge[20][2], int carrot[3], int side) {
    (*recursionDepth)++;

    /* Check side 1 (up) of the block for a same level carrot. */
    if ((*row != 0) && (grid[*row][*col] == grid[*row-1][*col]) && (side != 3)) {
        (*row)--;
        /* Record the coordinate to make zero later. */
        coordsToMerge[*mergeCondition][0] = *row;
        coordsToMerge[*mergeCondition][1] = *col;
        (*mergeCondition)++; /* Found one more same level carrot */
        mergeHelper(grid, gridSize, row, col, recursionDepth, mergeCondition, coordsToMerge,
            carrot, 1);
        (*row)++;
    }
    /* Check side 2 (right) of the block for a same level carrot. */
    if ((*col != gridSize - 1) && (grid[*row][*col] == grid[*row][*col + 1]) && (side != 4)) {
        (*col)++;
        coordsToMerge[*mergeCondition][0] = *row;
        coordsToMerge[*mergeCondition][1] = *col;
        (*mergeCondition)++;
        mergeHelper(grid, gridSize, row, col, recursionDepth, mergeCondition,
            coordsToMerge, carrot, 2);
        (*col)--;
    }
    /* Check side 3 (down) of the block for a same level carrot. */
    if ((*row != gridSize - 1) && (grid[*row][*col] == grid[*row + 1][*col]) && (side != 1)) {
        (*row)++;
        coordsToMerge[*mergeCondition][0] = *row;
        coordsToMerge[*mergeCondition][1] = *col;
        (*mergeCondition)++;
        mergeHelper(grid, gridSize, row, col, recursionDepth, mergeCondition,
            coordsToMerge, carrot, 3);
        (*row)--;
    }
    /* Check side 4 (left) of the block for a same level carrot. */
    if ((*col != 0) && (grid[*row][*col] == grid[*row][*col - 1]) && (side != 2)) {
        (*col)--;
        coordsToMerge[*mergeCondition][0] = *row;
        coordsToMerge[*mergeCondition][1] = *col;
        (*mergeCondition)++;
        mergeHelper(grid, gridSize, row, col, recursionDepth, mergeCondition,
            coordsToMerge, carrot, 4);
        (*col)++;
    }

    (*recursionDepth)--;

    /** Merging process **/
    if (*mergeCondition >= 2 && *recursionDepth == 0) {
        int i; /* Setting the values of merged carrots to 0. */
        for (i = 0; i < *mergeCondition; i++)
            grid[coordsToMerge[i][0]][coordsToMerge[i][1]] = 0;

        carrot[0] = carrot[0]+1;      /* Level up the carrot */
        *row = carrot[2];             /* Resetting row & col back to initial values */
        *col = carrot[1];
        grid[*row][*col] = carrot[0]; /* Place the new carrot */
        side = 0;
        *mergeCondition = 0;
        mergeHelper(grid, gridSize, row, col, recursionDepth, mergeCondition, coordsToMerge,
            carrot, side);  /* Recursive call again for further possible level-ups. */
    }
}


void merge(int grid[100][100], int carrot[3], int gridSize) {
    int mergeCondition = 0, recursionDepth = 0;
    /* Keeps track of the merging condition (2 more same numbers) */
    int *mergeConditionP = &mergeCondition;
    int side = 0;    /* Represents the direction. 1 = up, 2 = right, 3 = down,
                      * 4 = left. Using this variable, the recursion does not
                      * check the direction it came from. */
    int *recursionDepthP = &recursionDepth;/* Keeps track of the recursion depth. */
    int *row = &carrot[2], *col = &carrot[1];
    int coordsToMerge[20][2];
    grid[*row][*col] = carrot[0]; /* Place the carrot. */
    mergeHelper(grid, gridSize, row, col, recursionDepthP, mergeConditionP,
        coordsToMerge, carrot, side);
}

int main() {
    int gridSize;
    int grid[100][100]; /* grid[row][column] */
    int carrot[3]; /*carrot[0] = value, carrot[1] = column, carrot[2] = row*/
    scanf("%d", &gridSize);

    while (!feof(stdin)) {
        scanf("%d %d %d", &carrot[0], &carrot[1], &carrot[2]);
        grid[carrot[2]][carrot[1]] = carrot[0];
        merge(grid, carrot, gridSize);
    }

    printGrid(grid, gridSize);

    return 0;
}
