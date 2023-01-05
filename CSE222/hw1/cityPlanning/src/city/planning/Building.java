
package city.planning;

// Houses, Offices and Markets classes are inherited from Building class.
public class Building extends Structure{

    protected String owner;

    //constructors
    public Building(int height, int length, int position) {
        super(height, length, position);
    }

    public Building() {
        super();
    }

    // getter-setter
    public String getOwner() {
        return owner;
    }
    public void setOwner(String owner) {
        this.owner = owner;
    }
}
