package q4;

import java.io.Serializable;

/** Class for a binary tree that stores type E objects. */
public class BinaryTree<E> {

    /**
     * Class to encapsulate a tre node.
     * @param <E>
     */
    protected static class Node<E> implements Serializable {

        /** The information stored in this node. */
        protected E data;

        /** Reference to the right child. */
        protected Node<E> right;

        /** Reference to the left child. */
        protected Node<E> left;

        /**
         * Constructor a node with given data , left reference and right reference.
         * @param left left reference of node.
         * @param data  information stored in the node.
         * @param right right reference of node.
         */
        public Node( Node<E> left , E data , Node<E> right ) {
            this.left = left;
            this.data = data;
            this.right = right;
        }

        /** Construct a node with given data and no children.
         @param data The data to store in this node
         */
        public Node( E data ) {
            this( null , data , null );
        }

        /** Return a string representation of the node.
         @return A string representation of the data fields
         */
        @Override
        public String toString() {
            return data.toString();
        }
    }

    /** The root of the binary tree */
    protected Node<E> root;

    /**
     * No parameter constructor.
     */
    protected BinaryTree() {
        root = null;
    }

    /**
     *  Construct a node with given node as root.
     * @param root node that will be root of q3.BinaryTree.
     */
    protected BinaryTree( Node<E> root ) {
        this.root = root;
    }

    /**
     * Constructs a new binary tree with data in its root leftTree
     * as its left subtree and rightTree as its right subtree.
     * @param data data
     * @param left left reference
     * @param right right reference
     */
    protected BinaryTree( E data , BinaryTree<E> left , BinaryTree<E> right ) {

        root = new Node<>( data );

        if( left.root != null ) {
            root.left = left.root;
        }

        if( right.root != null ) {
            root.right = right.root;
        }

    }

    /** Return the left subtree.
     @return The left subtree or null if either the root or
     the left subtree is null
     */
    public BinaryTree<E> getLeftSubtree() {

        if( root != null && root.left != null ) {
            return new BinaryTree<>( root.left );
        }
        else {
            return null;
        }

    }

    /** Return the right subtree.
     @return The right subtree or null if either the root or
     the right subtree is null
     */
    public BinaryTree<E> getRightSubtree() {

        if( root != null && root.right != null ) {
            return new BinaryTree<>( root.right );
        }
        else {
            return null;
        }

    }

    /**
     * Return the data of the root node.
     * @return data which is hold in tree
     */
    public E getData() {
        if( root != null ) {
            return root.data;
        }

        System.out.println("Error : getData() root is null");
        return null;
    }

    /** Determine whether this tree is a leaf.
     @return true if the root has no children
     */
    public boolean isLeaf() {

        if( root != null ) {
            return ( root.left == null && root.right == null );
        }

        System.out.println("Error : root null in isLeaf ! ");
        return false;
    }

   
    protected void preOrderTraverse( Node<E> node , StringBuilder sb ) {

        if( node != null ) {
            sb.append( node.toString() ).append(" ");
            preOrderTraverse( node.left , sb );
            preOrderTraverse( node.right , sb );
        }
    }


    /**
     * Return prefix constructed string representation of the tree.
     * @return String representation of current tree.
     */
    @Override
    public String toString() {

        StringBuilder sb = new StringBuilder();
        preOrderTraverse( root , sb );
        return sb.toString();

    }
}