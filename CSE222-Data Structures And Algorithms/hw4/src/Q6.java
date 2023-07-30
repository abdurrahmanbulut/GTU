public class Q6 {

    static void runQ6(int size){
        dfs2(size);
    }

    static void print_map(int[][] map, int n) {
        for (int i = 0; i < 2*n+1; i++) {
            System.out.print('_');
        }
        System.out.print('\n');
        for (int i = 0; i < n; i++) {
            System.out.print("|");
            for (int j = 0; j < n; j++) {
                char c = (char) (map[i][j]/n + 65);
                System.out.print(c + "|");

            }
            System.out.print("\n");
        }
        for (int i = 0; i < n*2+1; i++) {
            System.out.print('-');
        }
        System.out.print('\n');
    }

    static void dfs2_rec(int n, int[][] map, int curRow, int curCol, int step) {
        map[curRow][curCol] = step;

        if (step == n*n - 1) {
            print_map(map, n);
            map[curRow][curCol] = -1;
            return;
        }

        if (curRow < n - 1 && map[curRow + 1][curCol] == -1) {
            dfs2_rec(n, map, curRow + 1, curCol, step + 1);
        }
        if (curRow > 0 && map[curRow - 1][curCol] == -1) {
            dfs2_rec(n, map, curRow - 1, curCol, step + 1);
        }
        if (curCol < n - 1 && map[curRow][curCol + 1] == -1) {
            dfs2_rec(n, map, curRow, curCol + 1, step + 1);
        }
        if (curCol > 0 && map[curRow][curCol - 1] == -1) {
            dfs2_rec(n, map, curRow, curCol - 1, step + 1);
        }

        map[curRow][curCol] = -1;
    }

    static void dfs2(int n) {
        int[][] map = new int[n][n];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                map[i][j] = -1;
            }
        }

        dfs2_rec(n, map, 0, 0, 0);
    }

}
