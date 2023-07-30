package q4;

public class BinarySearchTree<E extends Comparable<E>> extends BinaryTree<E> implements SearchTree<E>{

    E[] tree;
    public BinarySearchTree(int size) {
        tree = (E[]) new Comparable[size];
    }

    @Override
    public boolean remove(E target) {
        return false;
    }

    @Override
    public boolean add(Comparable d){
        int i = 0; //Counter to traverse the array.
        while (i < tree.length && tree[i] != null) { //End of Array or null location found.
            if (d.compareTo(tree[i]) < 0) //Incoming data is smaller than data present in the array.
                i = (2 * i) + 1; //Move to left side of the tree.
            else
                i = (2 * i) + 2; //Move to right side of the tree
        }
        tree[i] = (E) d;
        return false;
    }

    @Override
    public boolean contains(E target) {
        return false;
    }

    public E find(E d) {
            int i = 0; //Counter to traverse the array.
            boolean found = false;
            while (i < tree.length) { //End of Array or 'd' found.
                if (d.compareTo(tree[i]) < 0) //Data to be searched is smaller than data present in the array.
                    i = (2 * i) + 1; //Move to left side of the tree.
                else if (d.compareTo(tree[i]) > 0)
                    i = (2 * i) + 2; //Move to right side of the tree
                else {//'d' is same as tree[i] i.e. d found.
                    found = true;
                    break;
                }
            }
            if (found) //'d' found.
                System.out.println(d + " found at " + i + ".");
            else //'d' not found.
                System.out.println(d + " not found.");
        return d;
    }

    public int[] findDel(E d) {
        int[] indices = new int[2];
        boolean found = false;
        int c = 0; //Child
        int p = 0; //Parent
        while (c < tree.length && tree[c] != null) { //End of Array or 'd' found.
            if (d.compareTo(tree[c]) < 0) { //Data to be searched is smaller than data present in the array.
                p = c; //Parent index
                c = (2 * c) + 1; //Move to left side of the tree.
            }
            else if (d.compareTo(tree[c]) > 0) {
                p = c; //Parent index
                c = (2 * c) + 2; //Move to right side of the tree
            }
            else { //'d' is same as tree[i] i.e. d found.
                found = true;
                break;
            }
        }
        if (found) {
            indices[0] = p;
            indices[1] = c;
            return indices;
        }
        else
            return null;
    }

    public void delNoChild(int child_index) {
        tree[child_index] = null;
    }

    public void delOneChild(int child_index, int left, int right) {
        if (tree[right] == null) { //The one child is at the left
            tree[child_index] = tree[left]; //The place to be deleted is replaced by the immediate left node
            int right_temp = (2 * left) + 2; //A variable used to store the right side of the immediate left node
            int left_temp = (2 * left) + 1; //A variable used to store the left side of the immediate left node
            while (right_temp < tree.length && left_temp < tree.length) { //Until the end of tree
                tree[left] = tree[left_temp]; //Moving up one level on the left side
                left = left_temp; //A sort of pointer being updated for the left side
                tree[right] = tree[right_temp]; //Moving up one level on the right side
                right = right_temp; //A sort of pointer being updated for the right side

                right_temp = (2 * right) + 2; //Right-most node of updated right
                left_temp = (2 * left) + 1; //Left-most node of updated left
            }
            //Setting the last nodes as null as all the preceding nodes have moved up one level
            tree[left] = null;
            tree[right] = null;

        }
        else if (tree[left] == null) { //The one child is at the right
            tree[child_index] = tree[right]; //The place to be deleted is replaced by the immediate right node
            int right_temp = (2 * right) + 2; //A variable used to store the right side of the immediate right node
            int left_temp = (2 * right) + 1; //A variable used to store the left side of the immediate right node
            while (right_temp < tree.length && left_temp < tree.length) { //Until the end of tree
                tree[left] = tree[left_temp]; //Moving up one level on the left side
                left = left_temp; //A sort of pointer being updated for the left side
                tree[right] = tree[right_temp]; //Moving up one level on the right side
                right = right_temp; //A sort of pointer being updated for the right side

                right_temp = (2 * right) + 2; //Right-most node of updated right
                left_temp = (2 * left) + 1; //Left-most node of updated left
            }
            //Setting the last nodes as null as all the preceding nodes have moved up one level
            tree[left] = null;
            tree[right] = null;
        }
    }

    public E delete(E d) {

        if (findDel(d) == null) {
            System.out.printf("\nKey not found, can't delete.\nThe list remains unchanged\n");
            return null;
        }
        else {
            int[] ref = findDel(d);
            int c = ref[1];
            int right = (2 * c) + 2;
            int left = (2 * c) + 1;
            System.out.printf("\nDeleting " + tree[c] + "\n");
            if ((right > tree.length || left > tree.length) || (tree[left] == null && tree[right] == null)) {//No child case
                delNoChild(c);
            } else if ((tree[right] == null && tree[left] != null) || (tree[right] != null && tree[left] == null)) { //One child case
                delOneChild(c, left, right);
            } else { //Two Child Case
                int temp = right; //Move to the right of the node to be deleted
                int temp2 = 0;
                while (tree[temp] != null) { //Until the right is empty
                    temp2 = temp; //Because temp will be null at the end of the loop, a variable to store the previous value
                    temp = (2 * temp) + 1; //Move to the left
                }
                tree[c] = tree[temp2]; //The value at temp2 copied at the node to be deleted
                c = temp2; //Child updated to carry out deletion of no child/one child at temp2
                //right and left updated to carry out deletion at temp2
                right = (2 * c) + 2;
                left = (2 * c) + 1;
                if ((right > tree.length || left > tree.length) || (tree[left] == null && tree[right] == null)) { //No child case
                    delNoChild(c);
                } else if ((tree[right] == null && tree[left] != null) || (tree[right] != null && tree[left] == null)) { //One child case
                    delOneChild(c, left, right);
                }
            }
        }
        return d;
    }

    public void traverse(int i) { //In-order implementation
        if (i < tree.length && tree[i] != null) { //Root is not null
            traverse((2 * i) + 1); //Left side of the list
            System.out.print(tree[i] + " ");
            traverse((2 * i) + 2); //Right side of the list
        }
    }

}