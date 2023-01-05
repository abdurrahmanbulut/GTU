package city.planning;

public class House extends Building {
    private int rooms;
    private String color;

    //default constructor
    public House(){
        this.rooms = 3;
        this.color = "white";
        this.owner = "";
    }
    public House(int rooms, String color, String owner) {
        this.rooms = rooms;
        this.color = color;
        this.owner = owner;
    }

    //getters-setters
    public int getRooms() {
        return rooms;
    }

    public void setRooms(int rooms) {
        this.rooms = rooms;
    }

    public String getColor() {
        return color;
    }

    public void setColor(String color) {
        this.color = color;
    }

    public void focus(){
        System.out.println("owner:" + owner);
    }
}
