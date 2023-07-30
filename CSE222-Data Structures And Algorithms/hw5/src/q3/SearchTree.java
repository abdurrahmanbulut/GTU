package q3;

/**
 * Search Tree Interface.
 * @param <E> Generic type of implemented tree.
 */
public interface SearchTree<E> {

    /**
     * Add item to tree.
     * @param item item
     * @return true if added otherwise return false.
     */
    boolean add( E item );

    public E[] toArray(E[] array);

    public void fromArray(E[] array);

    public E[] getSortedContents(E[] array);
    /**
     * Check the existence of given target element in tree.
     * @param target data that will be checked.
     * @return true if exists otherwise return false.
     */
    boolean contains( E target );

    /**
     * Starter method find.
     * pre: The target object must implement
     * the Comparable interface.
     * @param target The COmparable obejct being sought.
     * @return The object , if found , otherwise null.
     */
    E find( E target );

    /**
     * Delete the given target from the tree.
     * @param target data that will be deleted from the tree.
     * @return deleted data otherwise null.
     */
    E delete( E target );

    /**
     * Remove given target element from the tree.
     * @param target Element that will be removed.
     * @return true if element removed otherwise return false.
     */
    boolean remove( E target );

    boolean add(Comparable d);
}