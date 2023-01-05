package city.planning;

import java.util.Date;


public class Market extends Building{
    private String openingTime;
    private String closingTime;

    //default constructor
    public Market(){
        openingTime = "08:00";
        closingTime = "21:00";
    }
    public Market(String openingTime, String closingTime){
        super();
        this.openingTime = openingTime;
        this.closingTime = closingTime;
    }

    public void focus(){
        System.out.println("Closing time: " + closingTime);
    }
}
