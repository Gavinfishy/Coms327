public class jassignment0 {
    static int totalPaths = 0;
    static final int[] xMoves = {2,2,1,1,-2,-2,-1,-1};
    static final int[] yMoves = {1,-1,2,-2,1,-1,2,-2};
    static int n = 6;

    static boolean validMove(int x, int y, int[][] solution) {
        return(x >= 0 && x < solution.length && y >= 0 && y < solution.length && solution[x][y] == -1);
    }

    static void solveKT() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                int[][] solution = new int[n][n];
                for (int x = 0; x < n; x++) {
                    for (int y = 0; y < n; y++) {
                        solution[x][y] = -1;
                    }
                }
                solution[i][j] = 0;
                solveKTUtil(i, j, 1, solution, xMoves, yMoves);
            }
        }
        System.out.println(totalPaths);
    }

    static void solveKTUtil(int x, int y, int movei, int[][] solution, int[] xMove, int[] yMove) {
        int k;
        int nextX;
        int nextY;
        if (movei == n*n) {
            printSolution(solution);
            solution[x][y] = -1;
            return;
        }
        for (k = 0; k < 8; k++) {
            nextX = x + xMove[k];
            nextY = y + yMove[k];
            if (validMove(nextX,nextY,solution)) {
                solution[nextX][nextY] = movei;
                solveKTUtil(nextX,nextY, movei + 1, solution, xMove, yMove);
                solution[nextX][nextY] = -1;
            }
        }
    }

    static void printSolution(int[][] solution) {
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                if (y == n-1 && x == n-1) {
                    System.out.println(solution[x][y] + 1);
                }
                else {
                    System.out.print(solution[x][y] + 1 + ", ");
                }
            }
        }
        System.out.println();
        totalPaths++;
    }

    public static void main(String args[]) {
        jassignment0.solveKT();
    }
}

