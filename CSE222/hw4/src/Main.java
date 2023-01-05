import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        System.out.println("************* Driver Code ************");
//      question 1
        System.out.println("\nQ1 \"as\" in \"asdasd fffff hhhhh eeee as\" with 3rd occurrence");
        System.out.println("It is on index of " + Q1.stringSearch("as", "asdasd fffff hhhhh eeee as", 3, 0));

//         question 2
        System.out.println("\nQ2 array: '1,2,3,4,5,6,7,8,9,10'  , between 1 and 7");
        int[] arr = new int[]{1,2,3,4,5,6,7,8,9,10};
        System.out.println("There are " + Q2.numOfItem(arr, 0, arr.length-1, 1, 7) + " item/s");

        // question 3
        System.out.println("\nQ3 array: '1,2,3,4,5,6,7,8,9,10', target:  5");
        int[] arr2 = new int[]{1,2,3,4,5,6,7,8,9,10};
        ArrayList<ArrayList<Integer>> listOfLists = new ArrayList<ArrayList<Integer>>();
        ArrayList<Integer> sub = new ArrayList<Integer>();
        System.out.println(Q3.printSubArrays(arr2, 0, 0,  5, listOfLists));

        // question 4
        System.out.println("\nQ4 is a Python function. So, you can run python code for that.");


        // question 5
        System.out.println("\nQ5 with size 4");
        Q5.runQ5(4);

//          question 6
        System.out.println("\nQ6 with size 3");
        Q6.runQ6(3);
        System.out.println("************* Driver Code Worked ************");
    }


}
