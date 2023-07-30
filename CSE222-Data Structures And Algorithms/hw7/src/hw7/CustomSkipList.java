package hw7;

import java.util.Arrays;
import java.util.Random;

public class CustomSkipList <E extends Comparable<E>> {


    protected SLNode<E> head;

    private int size;

    private int maxLevel;

    private int maxCap;

    static final double LOG2 = Math.log(2.0);

    static final int MIN = Integer.MIN_VALUE;

    private Random rand = new Random();

    public int getSize() {
        return size;
    }
    public int getMaxLevel() {
        return maxLevel;
    }

    @SuppressWarnings("unchecked")
    protected static class SLNode<E> {

        SLNode<E>[] links;
        E data;

        SLNode( int m , E data ) {
            links = (SLNode<E>[]) new SLNode[m];
            this.data = data;
        }

        public String toString(){

            return "Data = " + data + "\n" +
                    "Level Size : " + links.length + "\n";
        }
    }

    @SuppressWarnings({ "unchecked" })
    public CustomSkipList(){
        size = 0;
        maxLevel = 0;
        maxCap = computeMaxCap(maxLevel);
        head = new SLNode(maxLevel, MIN);
    }

    private int logRandom() {
        int r = rand.nextInt( maxCap );
        int k = (int) (Math.log(r + 1) / LOG2);
        if (k > maxLevel - 1) {
            k = maxLevel - 1;
        }
        return maxLevel - k;
    }

    @SuppressWarnings("unchecked")
    private SLNode<E>[] search(E target) {

        SLNode<E>[] pred = (SLNode<E>[]) new SLNode[maxLevel];

        SLNode<E> current = head;

        for(int i = current.links.length - 1; i >= 0; i--){

            while(current.links[i] != null
                    && current.links[i].data.compareTo(target) < 0) {
                current = current.links[i];
            }

            pred[i] = current;

        }

        return pred;
    }

    public E find( E target ) {

        SLNode<E>[] pred = search( target );

        if( pred.length != 0 && pred[0].links[0] != null
                && (pred[0].links[0].data.compareTo( target ) == 0) ) {
            return pred[0].links[0].data;
        } else {
            return null;
        }
    }

    private int computeMaxCap( int currentLevelSize ) {
        return (int) (Math.pow(2, currentLevelSize) - 1);
    }

    public boolean add(E item ){

        SLNode<E>[] pred = search(item);

        if( pred.length != 0 && pred[0].links[0] != null
                && (pred[0].links[0].data.compareTo( item ) == 0) ) {
            return false;
        }

        ++size;

        if( size > maxCap ){
            ++maxLevel;
            maxCap = computeMaxCap(maxLevel);
            head.links = Arrays.copyOf(head.links, maxLevel);
            pred = Arrays.copyOf(pred, maxLevel);
            pred[maxLevel - 1] = head;
        }

        SLNode<E> newNode = new SLNode<>( logRandom() , item);

        for(int i = 0; i < newNode.links.length; i++){
            newNode.links[i] = pred[i].links[i];
            pred[i].links[i] = newNode;
        }

        return true;
    }
    public boolean remove( E item ) {

        SLNode<E>[] pred = search( item );

        if( pred.length == 0 || pred[0].links[0] == null
                || pred[0].links[0].data.compareTo( item ) != 0 ) {
            return false;

        }

        size--;

        SLNode<E> deleteNode = pred[0];

        for(int i = 0; i < deleteNode.links.length; i++){

            if(pred[i].links[i] != null) {
                pred[i].links[i] = pred[i].links[i].links[i];
            }

        }

        return true;
    }

    public String toString( ) {

        if( getSize() == 0 )
            return "No such element in the skip-list";

        StringBuilder str = new StringBuilder();

        SLNode<E> tempHead = head;

        for( int i = 0 ; i < size ; ++i  ) {

            tempHead = tempHead.links[0];
            str.append( tempHead.toString() ).append("\n");

        }

        return str.toString();
    }


}