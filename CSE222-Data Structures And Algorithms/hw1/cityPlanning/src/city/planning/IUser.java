package city.planning;


//interface for user operations out of edit and view mode actions.
public interface IUser {
    // It gets structure and its position, length and height and sets the structure properties.
    public void setPositionLengthHeightOfBuilding(Structure structure, int position, int len, int height);

    // it set length of street
    public void setLengthOfStreet(Street street, int lengthOfStreet);
}
