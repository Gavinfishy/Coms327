#include <stdbool.h>
#include <stdio.h>


void solveKTF(int x, int y, int move, int solutionSize, int solution[solutionSize][solutionSize], const int xMove[8], const int yMove[8]);
void printSolution(int solutionSize, int solution[solutionSize][solutionSize]);

static int totalPaths = 0;
static const int xMoves[8] = {2,2,1,1,-2,-2,-1,-1};
static const int yMoves[8] = {1,-1,2,-2,1,-1,2,-2};
static int n = 5;

bool validMove(int x, int y, int solutionSize, int solution[solutionSize][solutionSize]) {
    return ( x >= 0 && x < solutionSize && y >= 0 && y < solutionSize && solution[x][y] == -1);
}

void solveKT() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int solution[n][n];
            for (int x = 0; x < n; x++) {
                for (int y = 0; y < n; y++) {
                    solution[x][y] = -1;
                }
            }
            solution[i][j] = 0;
            solveKTF(i, j, 1, n, solution, xMoves, yMoves);
        }
    }
    printf("%d\n", totalPaths);
}

void solveKTF(int x, int y, int move, int solutionSize, int solution[solutionSize][solutionSize], const int xMove[8], const int yMove[8]) {
    int k;
    int nextX;
    int nextY;
    if (move == n*n) {
        printSolution(solutionSize, solution);
        solution[x][y] = -1;
        return;
    }
    for (k = 0; k < 8; k++) {
        nextX = x + xMove[k];
        nextY = y + yMove[k];
        if (validMove(nextX,nextY,solutionSize,solution)) {
            solution[nextX][nextY] = move;
            solveKTF(nextX,nextY, move + 1, solutionSize, solution, xMove, yMove);
            solution[nextX][nextY] = -1;
        }
    }
}

void printSolution(int solutionSize, int solution[solutionSize][solutionSize]) {
    for (int y = 0; y < n; y++) {
        for (int x = 0; x < n; x++) {
            if (y == n-1 && x == n-1) {
                printf("%d\n", solution[x][y] + 1);
            }
            else {
                printf("%d, ", solution[x][y] + 1);
            }
        }
    }
    totalPaths++;
}

int main() {
    solveKT();
    return 0;
}




