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

/*3456789012345678901234567890123456789012345678901234567890123456789012345678*/

void merge(int grid[100][100], int carrot[3], int gridSize) {
    int row = carrot[2], col = carrot[1], mergeCondition = 0;
    int i;
    int coordsToMerge[8][2];

    while (1){
        if (col != gridSize-1 && grid[row][col] == grid[row][col+1]) { /* Right */
            coordsToMerge[mergeCondition][0] = row;
            coordsToMerge[mergeCondition][1] = col+1;
            mergeCondition++;

            if (col != gridSize-2 && grid[row][col+1] == grid[row][col+2]) { /* right*/
                coordsToMerge[mergeCondition][0] = row;
                coordsToMerge[mergeCondition][1] = col+2;
                mergeCondition++;
            }
            else if (row != 0 && grid[row][col+1] == grid[row-1][col+1]) { /* up */
                coordsToMerge[mergeCondition][0] = row-1;
                coordsToMerge[mergeCondition][1] = col+1;
                mergeCondition++;
            }
            else if (row != gridSize-1 && grid[row][col+1] == grid[row+1][col+1]) { /* down */
                coordsToMerge[mergeCondition][0] = row+1;
                coordsToMerge[mergeCondition][1] = col+1;
                mergeCondition++;
            }
        }
        if (row != gridSize-1 && grid[row][col] == grid[row+1][col]) { /* Down */
            coordsToMerge[mergeCondition][0] = row+1;
            coordsToMerge[mergeCondition][1] = col;
            mergeCondition++;

            if (col != gridSize-1 && grid[row+1][col] == grid[row+1][col+1]) { /* right*/
                coordsToMerge[mergeCondition][0] = row+1;
                coordsToMerge[mergeCondition][1] = col+1;
                mergeCondition++;
            }
            else if (row != gridSize-2 && grid[row+1][col] == grid[row+2][col]) { /* down */
                coordsToMerge[mergeCondition][0] = row+2;
                coordsToMerge[mergeCondition][1] = col;
                mergeCondition++;
            }
            else if (col != 0 && grid[row+1][col] == grid[row+1][col-1]) { /* left */
                coordsToMerge[mergeCondition][0] = row+1;
                coordsToMerge[mergeCondition][1] = col-1;
                mergeCondition++;
            }
        }
        if (col != 0 && grid[row][col] == grid[row][col-1]) { /* Left */
            coordsToMerge[mergeCondition][0] = row;
            coordsToMerge[mergeCondition][1] = col-1;
            mergeCondition++;

            if (row != 0 && grid[row][col-1] == grid[row-1][col-1]) { /* up*/
                coordsToMerge[mergeCondition][0] = row-1;
                coordsToMerge[mergeCondition][1] = col-1;
                mergeCondition++;
            }
            else if (col != 1 && grid[row][col-1] == grid[row][col-2]) { /* left */
                coordsToMerge[mergeCondition][0] = row;
                coordsToMerge[mergeCondition][1] = col-2;
                mergeCondition++;
            }
            else if (row != gridSize-1 && grid[row][col-1] == grid[row+1][col-1]) { /* down */
                coordsToMerge[mergeCondition][0] = row+1;
                coordsToMerge[mergeCondition][1] = col-1;
                mergeCondition++;
            }
        }
        if (row != 0 && grid[row][col] == grid[row-1][col]) { /* Up */
            coordsToMerge[mergeCondition][0] = row-1;
            coordsToMerge[mergeCondition][1] = col;
            mergeCondition++;
            if (col != gridSize-1 && grid[row-1][col] == grid[row-1][col+1]) { /* right*/
                coordsToMerge[mergeCondition][0] = row-1;
                coordsToMerge[mergeCondition][1] = col+1;
                mergeCondition++;
            }
            else if (row != 1 && grid[row-1][col] == grid[row-2][col]) { /* up */
                coordsToMerge[mergeCondition][0] = row-2;
                coordsToMerge[mergeCondition][1] = col;
                mergeCondition++;
            }
            else if (col != 0 && grid[row-1][col] == grid[row-1][col-1]) { /* left */
                coordsToMerge[mergeCondition][0] = row-1;
                coordsToMerge[mergeCondition][1] = col-1;
                mergeCondition++;
            }
        }

        if (mergeCondition >= 2) {
            for (i = 0; i < mergeCondition; i++)
                grid[coordsToMerge[i][0]][coordsToMerge[i][1]] = 0;
            mergeCondition = 0;
            grid[row][col] = grid[row][col] + 1;
            continue;
        }
        break;
    }
}


int main() {
    int gridSize;
    int grid[100][100]; /* grid[row][column] */
    int carrot[3]; /*carrot[0] = value, carrot[1] = column, carrot[2] = row*/
    scanf("%d", &gridSize);

    while (!feof(stdin)) {
        if (feof(stdin))
            break;
        scanf("%d %d %d ", &carrot[0], &carrot[1], &carrot[2]);
        /*printf("\n--%d %d %d , grid before merge:\n", carrot[0], carrot[1], carrot[2]);*/
        /*printGrid(grid, gridSize);*/
        grid[carrot[2]][carrot[1]] = carrot[0];
        merge(grid, carrot, gridSize);

    }


    printGrid(grid, gridSize);

    return 0;
}
