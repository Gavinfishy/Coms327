#include <stdbool.h>
#include <stdio.h>
#define n 5

void solveKTF(int x, int y, int move, int solutionSize, int solution[solutionSize][solutionSize], const int xMove[8], const int yMove[8], int path[n*n]);
void printSolution(int solutionSize, int path[]);

static const int xMoves[8] = {2,2,1,1,-2,-2,-1,-1};
static const int yMoves[8] = {1,-1,2,-2,1,-1,2,-2};

bool validMove(int x, int y, int solutionSize, int solution[solutionSize][solutionSize]) {
    return ( x >= 0 && x < solutionSize && y >= 0 && y < solutionSize && solution[x][y] == -1);
}

void solveKT() {
    int path[n * n] = { };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int solution[n][n];
            for (int x = 0; x < n; x++) {
                for (int y = 0; y < n; y++) {
                    solution[x][y] = -1;
                }
            }
            solution[i][j] = 0;
            solveKTF(i, j, 1, n, solution, xMoves, yMoves,path);
        }
    }
}

void solveKTF(int x, int y, int move, int solutionSize, int solution[solutionSize][solutionSize], const int xMove[8], const int yMove[8], int path[]) {
    int k;
    int nextX;
    int nextY;
    if (move == n*n) {
        path[move - 1] = x*n + y + 1;
        printSolution(solutionSize, path);
        solution[x][y] = -1;
        return;
    }
    for (k = 0; k < 8; k++) {
        nextX = x + xMove[k];
        nextY = y + yMove[k];
        if (validMove(nextX,nextY,solutionSize,solution)) {
            path[move - 1] = x*n + y + 1;
            solution[nextX][nextY] = move;
            solveKTF(nextX,nextY, move + 1, solutionSize, solution, xMove, yMove, path);
            solution[nextX][nextY] = -1;
        }
    }
}

void printSolution(int solutionSize, int path[]) {
    for (int i = 0; i < n*n; i++) {
        if (i == (n*n - 1)) {
            printf("%d", path[i]);
        }
        else {
            printf("%d, ", path[i]);
        }
    }
    printf("\n");
}

int main() {
    solveKT();
    return 0;
}
