package city.planning;
import java.util.AbstractSequentialList;
import java.util.Iterator;
import java.util.ListIterator;
import java.util.NoSuchElementException;

/**
 * Linked List Class Implementation
 */

public class LDLinkedList<E> extends AbstractSequentialList<E> {
    private Node<E> head = null;
    private Node<E> tail = null;
    private Node<E> lazy = null;

    private int size = 0;


    /**
     * Get first element in the list.
     * @return Return first element in the list.
     */
    public E getFirst(){
        return head.data;
    }

    /**
     * Get last element in the list.
     * @return Return last element in the list.
     */
    public E getLast(){
        return tail.data;
    }
    /**
     * Return Iterator for List
     * @return an Iterator for List
     */
    public Iterator<E> iterator(){
        return new KWListIter(0);
    }


    public Node<E> search(Node<E> current, E item)
    {
        while (current != null)
        {
            if (current.data == item){
                return current;    //data found
            }
            current = current.next;
        }
        return null;    //data not found
    }

    /**
     * Remove item from linked list at the index
     * @param index position of removed item
     * @return return remove data
     */
    @Override
    public E remove(int index){
        if (head == null)
            return null;
        Node<E> temp = head;
        Node<E> control = null;

        if (index == 0) {

            //listte head.data yoksa
            control = search(lazy, head.data);
            if (control == null){
                addNode(lazy, head);
                remove(temp);
            }
            head = temp.next;
            size--;
            return temp.data;
        }

        for (int i = 0 ; temp != null && i < index - 1; i++) {
            temp = temp.next;
        }
        if (temp == null || temp.next == null)
            return null;
        if(index == size -1) {
            tail = temp;
        }

        control = search(lazy, head.data);
        if (control == null){
            addNode(lazy, head);
            remove(temp);
        }


        Node<E> next = temp.next.next;
        temp.next = next;
        size--;
        return temp.data;
    }

    public void addNode(Node<E> node, Node<E> newNode){

        if (node == null)
            node = newNode;

        while (node.next != null){
            node = node.next;
        }
        node.next = newNode;
    }

    public void removeNode(Node<E> node, Node<E> rmNode) {
        if(node == null)
            return;

        while (node.next.next != rmNode){
            node = node.next;
        }
        node.next = rmNode.next;
    }

    /**
     *
     * @param obj
     * @return
     */
    @Override
    public boolean remove(Object obj){
        for(int i = 0 ; i < size ; i++){
            if(get(i).equals(obj)) {
                remove(i);
                return true;
            }
        }
        return false;
    }
    /**
     * Return ListIterator for list
     * @return Return ListIterator for list
     */
    public ListIterator<E> listIterator(){
        return new KWListIter(0);
    }

    /** Return a ListIterator from begins at index
     * @param index - The position of iterator is to begin
     * @return ListIterator that begins at index
     */
    public ListIterator<E> listIterator(int index){
        return new KWListIter(index);
    }
    /**
     * Add an item at the specified index.
     * @param index index of the object is to be inserted
     * @param obj The object to be inserted
     */
    @Override
    public void add(int index, E obj){
        listIterator(index).add(obj);
    }

    /**
     * Get element at position index.
     * @param index Position of item
     * @return The item at index
     */
    @Override
    public E get(int index){
        return listIterator(index).next();
    }

    /**
     * Return size of the list
     * @return size of the list
     */
    @Override
    public int size() {
        return size;
    }
    /**
     * A Node private Class For Linked List
     */
    private static class Node<E> {
        public E data;
        public Node<E> next = null;
        public Node<E> prev = null;

        /**
         * Constructor with given data
         * @param dataItem Return data
         */
        private Node(E dataItem) {
            data = dataItem;
        }

    }
    private class KWListIter implements ListIterator<E> {
        private Node<E> nextItem;
        private Node<E> lastItemReturned;
        private int index = 0;
        /**
         * Constructor for KWListIter
         * @param i Index of the item to be iterate
         */
        public KWListIter(int i) {
            if (i < 0 || i > size) {
                throw new IndexOutOfBoundsException(
                        "Invalid index " + i);
            }
            lastItemReturned = null;
            if (i == size) {
                index = size;
                nextItem = null;
            } else {
                nextItem = head;
                for (index = 0; index < i; index++) {
                    nextItem = nextItem.next;
                }
            }
        }

        /**
         * Copy Constructor for KWListIter
         * @param other  Copied KWListIter
         */
        public KWListIter(KWListIter other) {
            KWListIter itr = new KWListIter(0);
            itr.index = other.index;
            itr.lastItemReturned = other.lastItemReturned;
            itr.nextItem = other.nextItem;
        }
        /**
         * Determine next item null or not
         * @return true if next item not null
         */
        @Override
        public boolean hasNext() {
            return nextItem != null;
        }

        /** Move the iterator return the next item.
         @return Return next item
         */
        @Override
        public E next() {
            if (!hasNext()) {
                throw new NoSuchElementException();
            }
            lastItemReturned = nextItem;
            nextItem = nextItem.next;
            index++;
            return lastItemReturned.data;
        }

        /**
         * Determine previous item null or not
         * @return true if previous item not null
         */
        @Override
        public boolean hasPrevious() {
            return (nextItem == null && size != 0)
                    || nextItem.prev != null;
        }
        /**
         * Return the index of the next item
         * @return the index of the next item
         */
        @Override
        public int nextIndex() {
            return index;
        }

        /**
         * Return the index of the previous item
         * @return the index of the previous item
         */
        @Override
        public int previousIndex() {
            return index - 1;
        }
        /**
         * Move the iterator return the previous item
         * @return Return next item
         */
        @Override
        public E previous(){
            if (!hasPrevious()){
                throw new NoSuchElementException();
            }
            if (nextItem == null){
                nextItem = tail;
            } else {
                nextItem = nextItem.prev;
            }
            lastItemReturned = nextItem;
            index--;
            return lastItemReturned.data;
        }
        /**
         * Add a new item to linked list
         * @param obj The item to be added
         */
        @Override
        public void add(E obj){
            Node<E> current = search(lazy, obj);

            if (head == null){
                if (current != null){
                    head = current;
                    removeNode(lazy, current);
                }else{
                    head = new Node<>(obj);
                }
                tail = head;
            }
            else if (nextItem == head){
                Node<E> newNode;

                if (current != null){
                    newNode = current;
                    removeNode(lazy, current);
                }else{
                    newNode = new Node<>(obj);
                }
                newNode.next = nextItem;
                nextItem.prev = newNode;
                head = newNode;
            }
            else if (nextItem == null){
                Node<E> newNode;
                if (current != null){
                    newNode = current;
                    removeNode(lazy, current);
                }else{
                    newNode = new Node<>(obj);
                }
                tail.next = newNode;
                newNode.prev = tail;
                tail = newNode;
            }
            else{
                Node<E> newNode;
                if (current != null){
                    newNode = current;
                    removeNode(lazy, current);
                }else{
                    newNode = new Node<>(obj);
                }
                newNode.prev = nextItem.prev;
                nextItem.prev.next = newNode;
                newNode.next = nextItem;
                nextItem.prev = newNode;
            }
            size++;
            index++;
            lastItemReturned = null;
        }

        /**
         *  Remove the last item returned
         */
        @Override
        public void remove() {
            if(head == null)
                return;
            if(lastItemReturned == head) {
                lastItemReturned.next = head;
                return;
            }
            if(lastItemReturned == tail) {
                lastItemReturned.prev = tail;
                return;
            }
            lastItemReturned.prev.next = lastItemReturned.next;
            lastItemReturned.next.prev = lastItemReturned.prev;
        }
        /** Set the last item returned
         *  @param item new data
         */
        @Override
        public void set(E item) {
            lastItemReturned.data = item;
        }
    }
}