package hw7;/* A program to convert Binary Tree to Binary Search Tree */

import java.util.*;

public class BinarySearchToBST{
    static int COUNT = 10;

    static class Node <E extends Comparable<E>>{
        E data;
        Node left;
        Node right;
        public Node(E data){
            //Assign data to the new node, set left and right children to null
            this.data = data;
            this.left = null;
            this.right = null;
        }
        public  Node(){
            this.left = null;
            this.right = null;
        }


    };

    // array index
    static int index;


    private static int countNodes(Node root)
    {
        if (root == null)
            return 0;
        return countNodes(root.left) + countNodes(root.right) + 1;
    }

    // Copy elemenyt of arr[] to Binary Tree.
    private static <E extends Comparable<E>> void arrayToBST(E[] arr, Node root)
    {
        if (root == null)
            return;

        arrayToBST(arr, root.left);

        root.data = arr[index];
        index++;

        arrayToBST(arr, root.right);
    }

    // Converts a Binary Tree to BST
    private static  <E extends Comparable<E>> void convertBTtoBST(Node root, E[] values)
    {
        // if tree is empty
        if (root == null)
            return;

        int n = countNodes(root);



        Arrays.sort(values);


        // Copy array elements back to Binary Tree
        index = 0;
        arrayToBST(values, root);
    }

    // print Binary Tree as inorder
    static void printInorder(Node node, int space)
    {
        if (node == null)
            return;

        space += COUNT;
        System.out.println(" ");
        printInorder(node.right, space);



        for (int i = COUNT; i < space; i++)
            System.out.print(" ");
        System.out.print(node.data + "\n");


        printInorder(node.left, space);


    }

    public static <E extends Comparable<E>> void drive(E[] values)
    {
        Node root = null;

		/*
                         23
                        /  \
                       45   7
                      /	 \
                    46	 14
	        I was able to show the version rotated 90 degrees counterclockwise.
	    */
        System.out.println("Binary tree structure is creating...");
        root = new Node(0);
        System.out.println("Head node created. Value : 23");
        root.left = new Node(0);
        System.out.println("left node created with value 45");
        root.right = new Node(0);
        System.out.println("right node created with value 7");
        root.left.left = new Node(0);
        System.out.println("left -> left node created with value 46");
        root.left.right = new Node(0);
        System.out.println("left -> right node created with value 14");



        System.out.println("Original Binary Tree with zero(s)");
        printInorder(root, 0);
        System.out.println();

        // A method to convert Binary Tree to BST
        convertBTtoBST(root, values);

        System.out.println("Converted to BST: ");
        printInorder(root, 0);

    }
}
