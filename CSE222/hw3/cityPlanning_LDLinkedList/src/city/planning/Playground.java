package city.planning;

public class Playground extends Structure{

    public Playground(){
        height = 5;
    } // height property is set to 5. This is not from building class. So, we can get height constant.

    public Playground(int length, int position) {
        super(5, length, position);
    }

    public void focus(){
        System.out.println("Length: " + length);
    }
}
