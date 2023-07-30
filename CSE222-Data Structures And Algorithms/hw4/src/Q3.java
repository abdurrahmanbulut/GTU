import java.util.ArrayList;
import java.util.List;

public class Q3 {

    public static ArrayList<ArrayList<Integer>> printSubArrays(int []arr, int start, int end, int target, ArrayList<ArrayList<Integer>> t)
    {
        if (end == arr.length){
            return t;
        }
        else if (start > end)
            return printSubArrays(arr, 0, end + 1, target, t);

        else
        {
            ArrayList<Integer> temp = new ArrayList<>();
            int sum = 0;
            for (int i = start; i <= end; i++){
                temp.add(arr[i]);
                sum += arr[i];
            }
            if (sum == target){
                t.add(temp);
            }
            return printSubArrays(arr, start + 1, end, target, t);
        }
    }
}
