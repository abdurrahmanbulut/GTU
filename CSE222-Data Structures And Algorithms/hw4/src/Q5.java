public class Q5 {

    static void runQ5(int size){
        dfs(size);
    }
    static void paint_n(char [] arr, char colour, int n, int start) {
        for (int i = start; i < start + n; i++) {
            arr[i] = colour;

        }
    }
    static void dfs_rec(int len, int cur, char [] s) {
        if (len - cur < 3) {
            return;
        }

        for (int j = 3; j <= len - cur; j++) {
            for (int i = cur; i <= len - j; i++) {

                for (int k = 0; k < len+2; k++) {
                    System.out.print("-");
                }

                paint_n(s, 'X', j, i);


                System.out.println();
                System.out.print("|");
                System.out.print(s);
                System.out.println("|");

                for (int k = 0; k < len+2; k++) {
                    System.out.print("-");
                }

                System.out.println();


                dfs_rec(len, cur + j + i + 1, s);
                paint_n(s, 'O', len - cur, cur);
            }
        }
    }

    static void dfs(int n) {
        char [] arr = new char[n];
        paint_n(arr, 'O', n, 0);
        dfs_rec(n, 0, arr);
    }
}
