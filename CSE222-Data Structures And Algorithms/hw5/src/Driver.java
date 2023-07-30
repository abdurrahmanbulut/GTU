import q3.BinaryHeap;
import q4.BinarySearchTree;

import java.sql.Array;
import java.util.ArrayList;

public class Driver {
    public static void main(String[] args) {



        BinaryHeap<Integer> bh1 = new BinaryHeap<>();
        System.out.println("Heap constructor worked");
        bh1.add(12);
        System.out.println("12 added to heap");
        bh1.add(44);
        System.out.println("44 added to heap");
        bh1.getLeftSubtree();
        bh1.getRightSubtree();
        bh1.remove(12);
        System.out.println("12 removed from heap");


        //************************
        BinarySearchTree<Integer> bst1 = new BinarySearchTree<>(6);
        System.out.println("Binary Search Tree constructor worked with size 6");
        bst1.add(15);
        System.out.println("15 added to BST");
        bst1.add(13);
        System.out.println("13 added to BST");
        bst1.delete(13);
        bst1.find(15);
        bst1.isLeaf();
        bst1.getLeftSubtree();
        bst1.getRightSubtree();




    }
}
