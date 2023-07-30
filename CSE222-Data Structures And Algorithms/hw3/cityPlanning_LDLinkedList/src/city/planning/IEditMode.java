package city.planning;

// An interface for edit mode.
public interface IEditMode {
    // this func. adds structure to street. If there is no empty land on street, it wil return false. Otherwise, it will return true.
    public boolean addBuilding( Street street, Structure structure, String side);
    // it deletes structure from street by looking the reference.
    public void deleteBuilding( Street street, String side, Structure structure);
}
