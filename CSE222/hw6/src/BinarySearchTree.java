import java.util.Iterator;
/** Binary Search Tree implementation.
 *   @author Abdurrahman BULUT
 * */
public class BinarySearchTree<E extends Comparable<E>>
        extends BinaryTree<E>
        implements SearchTree<E> {

    /**
     * Return value from the public add method.
     */
    protected boolean addReturn;

    /**
     *Return value from the public delete method.
     */
    protected E deleteReturn;

    /**
     * Check the existence of given target element in tree.
     * @param target data that will be checked.
     * @return true if exists otherwise return false.
     */
    @Override
    public boolean contains(E target) {

        E returnedValue = find( target );
        return returnedValue != null;
    }


    /**  Starter method add.
     * pre : The object to insert must implement the Comparable interface.
     * @param item item THe Obejct being insrted.
     * @return true if the object is inserted , false if the object alredy
     * exist in the tree.
     */
    @Override
    public boolean add(E item) {
        root = add( root , item);
        return false;
    }

    /**
     * Add a node an item to tree.
     * @param localRoot A starting node for finding appropriate subtree.
     * @param item data that will be inserted to tree.
     * @return added data otherwise return null.
     */
    private Node<E> add( Node<E> localRoot ,E item ) {

        if( localRoot == null ) {
            addReturn = true;
            return new Node<>( item );
        }
        else if( item.compareTo( localRoot.data) == 0 ) {
            addReturn = false;
            return localRoot;
        }
        else if( item.compareTo( localRoot.data ) < 0 ){
            localRoot.left = add( localRoot.left , item );
            return localRoot;
        }
        else {
            localRoot.right = add( localRoot.right , item);
            return localRoot;
        }
    }



    /**
     * Starter method find.
     * pre: The target object must implement
     * the Comparable interface.
     * @param target The COmparable obejct being sought.
     * @return The object , if found , otherwise null.
     */
    @Override
    public E find(E target) {
        return find( root , target );
    }

    /**
     * Recursive find method.
     * @param localRoot localRoot the local subtree's root.
     * @param target The object being sought
     * @return The object , if found , otherwise null.
     */
    protected E find(Node<E> localRoot , E target ) {

        if( localRoot == null ) {
            return null;
        }

        int compResult = target.compareTo( localRoot.data );

        if( compResult == 0 ) {
            return localRoot.data;
        }
        else if( compResult < 0 ) {
            return find( localRoot.left , target );
        }
        else {
            return find( localRoot.right , target );
        }

    }


    /**
     * Delete the given target from the tree.
     * @param target data that will be deleted from the tree.
     * @return deleted data otherwise null.
     */
    @Override
    public E delete(E target) {
        root = delete( root , target );

        return deleteReturn;
    }

    /**
     * Delete the the given data , return the corresponding node.
     * @param localRoot A starting node for searching node throughout the tree.
     * @param target Data that will be deleted from the tree.
     * @return deleted node otherwise return null.
     */
    private Node<E> delete( Node<E> localRoot , E target ) {

        if( localRoot == null ) {
            return null;
        }
        int compResult = target.compareTo( localRoot.data );

        if( compResult < 0 ) {
            localRoot.left = delete( localRoot.left , target );
            return localRoot;
        }
        else if( compResult > 0 ) {
            localRoot.right = delete( localRoot.right , target );
            return localRoot;
        }
        else {
            deleteReturn = localRoot.data;

            if( localRoot.left == null ) {
                return localRoot.right;
            }
            else if( localRoot.right == null ) {
                return localRoot.left;
            }
            else {
                if( localRoot.left.right == null ) {
                    localRoot.data = localRoot.left.data;
                    localRoot.left = localRoot.left.left;
                }
                else {
                    localRoot.data = findLargestChild( localRoot.left);
                }
                return localRoot;
            }
        }
    }

    /**
     * Find the node that is the inorder predecessor and replace it
     * with its left child( if any)
     * post : The inorder predecessor is removed form the tree.
     * @param parent The parent of possible inorder
     *               predecessor(ip)
     * @return The data in the ip
     */
    private E findLargestChild( Node<E> parent ) {
        if( parent.right.right == null ) {
            E returnedValue = parent.right.data;
            parent.right = parent.right.left;
            return returnedValue;
        }
        else {
            return findLargestChild( parent.right);
        }
    }

    /**
     * Remove given target element from the tree.
     * @param target Element that will be removed.
     * @return true if element removed otherwise return false.
     */
    @Override
    public boolean remove(E target) {
        return delete(target) != null;
    }

}