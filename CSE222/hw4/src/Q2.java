public class Q2 {

    public static int numOfItem(int[] arr, int l, int r, int l_point, int r_point){

        if (arr[0] > l_point || arr[arr.length-1] < r_point) return -1;
        if (l > r)
            return 0;

        int mid = l +(r-l)/2;
        int count  = 0;

        if (arr[mid] <= r_point && arr[mid] >= l_point)
            count=1;

        int right_side = 0;
        int left_side = 0;


        if(count == 1 || arr[mid] < l_point)
            right_side = numOfItem(arr, mid+1, r, l_point, r_point);
        if(count == 1 || arr[mid] > r_point)
            left_side = numOfItem(arr, l, mid-1, l_point, r_point);


        return left_side + right_side + count;
    }
}
