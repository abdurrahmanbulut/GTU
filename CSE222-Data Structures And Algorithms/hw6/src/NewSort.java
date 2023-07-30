public class NewSort {

    public static < T
            extends Comparable < T >> void sort(T[] table){
        new_sort(table, 0,table.length-1);
    }

    private static < T extends Comparable < T >> T[] new_sort(T[] table, int first, int last) {
        if (first > last) return table;
        else {
            Pair temp = new Pair();
            min_max_finder(table, first, last, temp);
            int min = temp.min;
            int max = temp.max;
            swap(table, first, table, min);
            swap(table, last, table, max);
            return new_sort(table, first+1, last-1);

        }
    }
    private static < T
            extends Comparable < T >> void swap(T[] a, int indexA, T[] b, int indexB){
        T temp = a[indexA];
        a[indexA] = b[indexB];
        b[indexB] = temp;
    }

    private static < T
            extends Comparable < T >> void min_max_finder(T[] nums, int left, int right, Pair p){
        if (left == right)                  // common comparison
        {
            if ((Integer)nums[p.max] < (Integer)nums[left]) {          // comparison 1
                p.max = left;
            }

            if ((Integer)nums[p.min] > (Integer)nums[right]) {         // comparison 2
                p.min = right;
            }
            return;
        }

        // if the array contains only two elements
        if (right - left == 1)              // common comparison
        {
            if ((Integer)nums[left] < (Integer)nums[right])         // comparison 1
            {

                if ((Integer)nums[p.min] > (Integer)nums[left]) {      // comparison 2
                    p.min = left;

                }

                if ((Integer)nums[p.max] < (Integer)nums[right]) {     // comparison 3
                    p.max = right;
                }
            }
            else {
                if ((Integer)nums[p.min] > (Integer)nums[right]) {     // comparison 2
                    p.min = right;
                }

                if ((Integer)nums[p.max] < (Integer)nums[left]) {      // comparison 3
                    p.max = left;
                }
            }

            return;
        }

        // find the middle element
        int mid = left + (right - left)/2;

        // recur for the left subarray
        min_max_finder(nums, left, mid, p);

        // recur for the right subarray
        min_max_finder(nums, mid + 1, right, p);
    }
    public static class Pair {
        int min;   // the smaller one
        int max; // the bigger one
        Pair ( int min, int max ) { this.min = min; this.max = max; }
        Pair () {min = 0; max = 0;}

    }

}
