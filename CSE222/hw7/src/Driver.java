import hw7.BinarySearchToBST;
import hw7.BinarySearchTree;
import hw7.CustomSkipList;

public class Driver {
    public static void main(String[] args) {
        System.out.println("** HW7 DRIVER CODE **");
        System.out.println("** Question 1 Start**");


                 /*        23
                        /  \
                       45   7
                      /	 \
                    46	 14*/
	       /* I was able to show the version rotated 90 degrees counterclockwise in console.*/
        System.out.println("Test 1");
        Integer[] values = new Integer[]{23, 45, 7, 46, 14};
        Integer[] values2 = new Integer[]{7, 3, 1, 28, 55};
        System.out.println("integer array created with values [23, 45, 7, 46, 14]");
        BinarySearchToBST.drive(values);
        System.out.println("I was able to show the version rotated 90 degrees counterclockwise in console.");
        System.out.println("It keeps the original structure of Binary Tree when converting BST");
        System.out.println("Test 2");
        BinarySearchToBST.drive(values2);
        System.out.println("** Question 1 END **");
        System.out.println();
        System.out.println("** Question 2 Start**");

        System.out.println("Test 1");
        BinarySearchTree<Integer> bst = new BinarySearchTree<>();
        System.out.println("Binary search tree created");
        bst.add(12);
        System.out.println("12 added to BST");
        bst.add(23);
        System.out.println("23 added to BST");
        bst.add(66);
        System.out.println("66 added to BST");
        bst.add(1);
        System.out.println("1 added to BST");
        bst.add(4);
        System.out.println("4 added to BST");
        bst.add(95);
        System.out.println("95 added to BST");
        bst.add(63);
        System.out.println("63 added to BST");
        bst.add(33);
        System.out.println("33 added to BST");
        bst.add(34);
        System.out.println("34 added to BST");
        bst.add(35);
        System.out.println("35 added to BST");
        System.out.println("Created BST");
        bst.drive();
        System.out.println("BST is converting to AVL tree");


        System.out.println("Test 2 | extended version of previous one");

        bst.add(12);
        System.out.println("12 added to BST");
        bst.add(23);
        System.out.println("23 added to BST");
        bst.add(66);
        System.out.println("66 added to BST");
        bst.add(1);
        System.out.println("1 added to BST");
        bst.add(8);
        System.out.println("8 added to BST");
        bst.add(6);
        System.out.println("6 added to BST");
        bst.add(156);
        System.out.println("156 added to BST");
        bst.add(28);
        System.out.println("28 added to BST");
        bst.add(33);
        System.out.println("33 added to BST");
        bst.add(38);
        System.out.println("38 added to BST");
        bst.add(24);
        System.out.println("24 added to BST");
        bst.add(5);
        System.out.println("5 added to BST");
        System.out.println("Created BST");
        bst.drive();
        System.out.println("BST is converting to AVL tree");

        System.out.println("** Question 2 End**");
        System.out.println();

        System.out.println("** Question 3 Start**");
        System.out.println();

        CustomSkipList<Integer> slist = new CustomSkipList<>();
        System.out.println("Skip list created");
        slist.add(12);
        System.out.println("12 added");
        slist.add(44);
        System.out.println("44 added");
        slist.add(66);
        System.out.println("66 added");
        slist.add(9);
        System.out.println("9 added");
        slist.add(4);
        System.out.println("4 added");
        slist.add(2);
        System.out.println("2 added");
        slist.add(35);
        System.out.println("35 added");
        slist.add(63);
        System.out.println("63 added");
        slist.add(27);
        System.out.println("27 added");
        slist.add(53);
        System.out.println("53 added");
        slist.add(92);
        System.out.println("92 added");
        slist.add(82);
        System.out.println("82 added");
        System.out.println("size: " + slist.getSize());
        System.out.println("max level: " + slist.getMaxLevel());
        System.out.println("** Question 3 End**");
        System.out.println();



    }
}
