package city.planning;

// An interface for viewing mode
public interface IViewMode {
    // it display the total remaining length of lands on the street.
    public void totalRemainingLengthOfLands(Street street);

    //display the list of buildings on the street.
    public void listBuildingsOnTheStreet(Street street);

    //display the number and ratio of lenth of playgrounds in the street.
    public void numberAndRatioOfPlaygrounds(Street street);

    //calculate the total length of street occupied by the markets, houses or offices.
    public void totalLengthOfBuildings(Street street);

    //display the skyline silhouette of the street.
    public void skylineSilhouette(Street street);
}
