package hw7;

public interface SearchTree < E
    extends Comparable < E >> {
  /** Inserts item where it belongs in the tree.
      @param item The item to be inserted
      @return true If the item is inserted, false if the
      item was already in the tree.
   */
  boolean add(E item);

}
