package hw7;

public class BinarySearchTree < E
        extends Comparable < E >>
        extends BinaryTree< E >
        implements SearchTree< E > {
  private static int COUNT = 10;
  // Data Fields
  protected boolean addReturn;
  protected E deleteReturn;




  //Methods

  public boolean add(E item) {
    root = add(root, item);
    return addReturn;
  }

  private Node < E > add(Node < E > localRoot, E item) {
    if (localRoot == null) {
      addReturn = true;
      return new Node < E > (item);
    }
    else if (item.compareTo(localRoot.data) == 0) {
      addReturn = false;
      return localRoot;
    }
    else if (item.compareTo(localRoot.data) < 0) {

      localRoot.left = add(localRoot.left, item);
      return localRoot;
    }
    else {
      localRoot.right = add(localRoot.right, item);
      return localRoot;
    }
  }

  public int getHeight(Node<E> node) {

    return (node==null) ? -1 : Math.max(getHeight(node.left),getHeight(node.right)) + 1;

  }


  public int getBalance(Node<E> root) {
    int leftHeight;
    int rightHeight;


    leftHeight = getHeight(root.left);
    rightHeight = getHeight(root.right);
    return (leftHeight-rightHeight);
  }


  protected Node < E > rotateRight(Node < E > A) {
    Node < E > B = A.left;
    A.left = B.right;

    B.right=getRotation(A);

    return getRotation(B);
  }

  protected Node < E > rotateLeft(Node < E > A) {
    Node<E> B = A.right;
    A.right = B.left;

    B.left = getRotation(A);

    return getRotation(B);
  }

  public Node < E > getRotation(Node < E > root) {


    if (getBalance(root) >= 2) { // ll,lr
      if (getBalance(root.left) >= 1) { // ll
        return rotateRight(root);
      } else { // lr
        root.left = rotateLeft(root.left);
        return rotateRight(root);
      }

    } else if (getBalance(root) <= -2) { // rr,rl
      if (getBalance(root.right) <= -1) { // rr
        return rotateLeft(root);
      } else { // rl
        root.right = rotateRight(root.right);
        return rotateLeft(root);
      }
    }
    return root;
  }

  public Node < E > balanceBST(Node < E > root) {
    if (root == null)
      return null;
    root.left = balanceBST(root.left);
    root.right = balanceBST(root.right);


    return getRotation(root);
  }

  public void drive(){
    System.out.println("*****************************************************************");
    printInorder(root, 0);
    System.out.println();
    System.out.println("*****************************************************************");
    root = balanceBST(root);

    printInorder(root, 0);
    System.out.println("*****************************************************************");

    System.out.println(getHeight(root));
    System.out.println(getBalance(root));
  }


  // print Binary Tree  as inorder
  public void printInorder(BinaryTree.Node<E> localRoot, int space)
  {
    if (localRoot == null)
      return;

    space += COUNT;
    System.out.println(" ");
    printInorder(localRoot.right, space);



    for (int i = COUNT; i < space; i++)
      System.out.print(" ");
    System.out.print(localRoot.data + "\n");


    printInorder(localRoot.left, space);


  }
}
