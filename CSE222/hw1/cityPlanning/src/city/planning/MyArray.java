package city.planning;

import java.security.PublicKey;

public class MyArray<T> {

    private int size;
    private int initialCapacity;
    protected Object[] array;

    public MyArray() {
        size = 0;
        initialCapacity = 10; // initial capacity is set to 10. İf it needs to increase, it will be +10 size.
        array = new Object[initialCapacity];

    }
    public void setSize(int size) {
        this.size = size;
    }

    public void setInitialCapacity(int initialCapacity) {
        this.initialCapacity = initialCapacity;
    }

    public void setArray(Object[] array) {
        this.array = array;
    }

    public Object[] getArray() {
        return array;
    }

    public int getSize() {
        return size;
    }

    public int getInitialCapacity() {
        return initialCapacity;
    }

    public Object getArrayElement(int index){
        return array[index];
    }

    // if capacity is not enough to store data, it increase capacity by 10.
    public boolean add (T newObject) {
        if (array == null)
            array = new Object[initialCapacity];

        if (this.initialCapacity <= size) {
            initialCapacity+=10;
            Object[] newArray = new Object[initialCapacity];
            for(int i = 0; i < size; i++)
                newArray[i] = array[i]; // copy array to new increased array

            array= (Object[]) newArray;
        }
        array[size]=newObject;
        size++;

        return true;
    }

    public  void delete (T value) {
        int index = 0;
        for(int i=0; i<size; i++) {
            if (value == array[i])
                index = i;
        }
        for ( int i = index; i!= size; i++)
            array[i]=array[i+1];
        --size;
    }


    @Override
    public String toString(){
        String allElements = " ";
        for (int i = 0 ; i < size; i++){
            allElements += array[i]+ " ";
        }
        return allElements;
    }
}