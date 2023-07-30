import city.planning.*;

import java.sql.SQLOutput;
import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.Locale;
import java.util.Scanner;

// This is our Driver class to run code.
// Firstly driver code is run. After that a menu appears.
public class Driver {
    public static void main(String[] args) {

        boolean flag = true;
        Scanner scanner = new Scanner(System.in);
        driver();
        System.out.println("Driver Function is worked!");

        while (flag){

            System.out.println("Press 1 to go to menu");
            System.out.println("Press 2 to exit");

            int option;
            // exception for input mismatch
            try {
                option = scanner.nextInt();

                if (option == 1){
                    menu();
                }
                else if(option == 2){
                    System.out.println("Goodbye!");
                    flag = false;
                }
                else{
                    System.out.println("Invalid option!");
                }
            }
            catch (InputMismatchException e){
                System.out.println("Input mismatch exception!");
                scanner.next();
            }
        }

    }
    // Menu func let create and modify buildings on street step by step.
    public static void menu(){
        Scanner scanner = new Scanner(System.in);
        Street street = new Street();
        User user = new User();

        boolean flag = true;

        //for first step, it shows modes. If street is not iitialized, view mode will go back.
        while (flag){
            System.out.println("Press 1 to go to Edit mode.");
            System.out.println("Press 2 to go to View mode.");
            System.out.println("Press 3 to Exit.");

            int option=0;

            // exception for input mismatch
            try {
                option = scanner.nextInt();
            }
            catch (InputMismatchException e){
                System.out.println("Input mismatch exception!");
                scanner.next();
            }

            // Step2: Edit mode option
            switch(option) {
                case 1:
                    System.out.println("********* Edit Mode *************");

                    if (street.getLengthOfStreet() == 0){
                        System.out.println("Street was not created.");
                        System.out.println("Please enter street length: ");
                        boolean createStreetflag = true;
                        while (createStreetflag){
                            // Exception for input mismatch
                            try {
                                int streetLen = scanner.nextInt();
                                street.setLengthOfStreet(streetLen);
                                createStreetflag = false;
                            }catch (InputMismatchException e){
                                System.out.println("Input mismatch exception is occured. Enter valid number!");
                                scanner.next();
                            }
                        }

                    }
                    boolean editFlag = true;
                    while (editFlag){
                        // Step: 3 add-delete building
                        int editOption=0;
                        System.out.println("Press 1 to add a building.");
                        System.out.println("Press 2 to delete a building.");
                        System.out.println("Press 3 to Exit.");
                        // Exception for input mismatch
                        try {
                            editOption = scanner.nextInt();
                        }
                        catch (InputMismatchException e){
                            System.out.println("Input mismatch exception!");
                            scanner.next();
                        }

                        // step4: building selection which will be created.
                        switch (editOption){
                            case 1:
                                boolean buildingTypeFlag=true;

                                while (buildingTypeFlag){
                                    int buildingTypeOption=0;
                                    System.out.println("1) Create Market");
                                    System.out.println("2) Create House");
                                    System.out.println("3) Create Office");
                                    System.out.println("4) Create Playground");
                                    System.out.println("5) Go back");
                                    // Exception fo input mismatch
                                    try {
                                        buildingTypeOption = scanner.nextInt();
                                        scanner.nextLine();
                                    }
                                    catch (InputMismatchException e){
                                        System.out.println("Input mismatch exception!");
                                        scanner.next();
                                    }
                                    // Market Creation
                                    if (buildingTypeOption == 1){
                                        System.out.println("Enter market opening time: (ex. 9:30AM)");
                                        String openingTime = scanner.nextLine();
                                        System.out.println("Enter market closing time: (ex. 9:30PM)");
                                        String closingTime = scanner.nextLine();
                                        Market market = new Market(openingTime, closingTime);

                                        int posOfMarket;
                                        int lenOfMarket;
                                        int heightOfMarket;
                                        String sideOfStreet;
                                        boolean flagMarket = true;
                                        while (flagMarket){
                                            try {
                                                System.out.println("Enter market position, length, height and side accordingly: (ex. 3 5 6 left) ");
                                                posOfMarket = scanner.nextInt();
                                                lenOfMarket = scanner.nextInt();
                                                heightOfMarket = scanner.nextInt();
                                                scanner.nextLine(); // to get "enter" character
                                                sideOfStreet = scanner.nextLine();

                                                // string check. It force to enter valid word.
                                                if(!(sideOfStreet.equals("left")) && !(sideOfStreet.equals("LEFT"))&& !(sideOfStreet.equals("right"))&& !(sideOfStreet.equals("RIGHT"))){
                                                    System.out.println("left, LEFT, right, RIGHT are acceptable words. Try again!");
                                                }
                                                else {
                                                    sideOfStreet = sideOfStreet.toLowerCase(Locale.ROOT);
                                                    if (posOfMarket<0 || posOfMarket > street.getLengthOfStreet() || posOfMarket+lenOfMarket > street.getLengthOfStreet()){
                                                        System.out.println("Out of street bound. Try again!");
                                                    }else{
                                                        market.setPositionLengthHeight(posOfMarket, lenOfMarket, heightOfMarket);
                                                        // if addBuilding returns true, it means process is succesful.
                                                        // otherwise, it means position of steret is not empty.
                                                        if (user.addBuilding(street, market, sideOfStreet)){
                                                            flagMarket = false;
                                                        }
                                                        else{
                                                            System.out.println("That position is not on empty land area. Try again!");
                                                        }
                                                    }
                                                }

                                            } catch (InputMismatchException e){
                                                System.out.println("Input mismatch exception occured. Try again!");
                                                scanner.next(); // to handle infinite loop
                                            }
                                        }
                                        // House Creation
                                    }else if(buildingTypeOption == 2){

                                        System.out.println("Enter number of rooms: (5)");
                                        int numberOfRooms = scanner.nextInt();
                                        scanner.nextLine();
                                        System.out.println("Enter house owner: (ex. Ali jack)");
                                        String ownerOfHouse = scanner.nextLine();
                                        System.out.println("Enter house color: (ex. Black)");
                                        String colorOfHouse = scanner.nextLine();
                                        House house = new House(numberOfRooms, colorOfHouse, ownerOfHouse);

                                        int posOfHouse;
                                        int lenOfHouse;
                                        int heightOfHouse;
                                        String sideOfStreet;
                                        boolean flagHouse = true;
                                        while (flagHouse){
                                            try {
                                                System.out.println("Enter house position, length, height and side accordingly: (ex. 3 5 6 left) ");
                                                posOfHouse = scanner.nextInt();
                                                lenOfHouse = scanner.nextInt();
                                                heightOfHouse = scanner.nextInt();
                                                scanner.nextLine();
                                                sideOfStreet = scanner.nextLine();

                                                if(!(sideOfStreet.equals("left")) && !(sideOfStreet.equals("LEFT"))&& !(sideOfStreet.equals("right"))&& !(sideOfStreet.equals("RIGHT"))){
                                                    System.out.println("left, LEFT, right, RIGHT are acceptable words. Try again!");
                                                }else{
                                                    sideOfStreet = sideOfStreet.toLowerCase(Locale.ROOT);
                                                    if (posOfHouse<0 || posOfHouse > street.getLengthOfStreet() || posOfHouse+lenOfHouse > street.getLengthOfStreet()){
                                                        System.out.println("Out of street bound. Try again!");
                                                    }else{
                                                        house.setPositionLengthHeight(posOfHouse, lenOfHouse, heightOfHouse);
                                                        if (user.addBuilding(street, house, sideOfStreet)){
                                                            flagHouse = false;
                                                        }
                                                        else{
                                                            System.out.println("That position is not on empty land area. Try again!");
                                                        }
                                                    }
                                                }

                                            } catch (InputMismatchException e){
                                                System.out.println("Input mismatch exception occured. Try again!");
                                                scanner.next();
                                            }
                                        }
                                        // Office Creation
                                    }else if(buildingTypeOption == 3){

                                        System.out.println("Enter job type: (software)");
                                        String  jobType = scanner.nextLine();
                                        System.out.println("Enter house owner: (ex. Ali jackie)");
                                        String ownerOfOffice = scanner.nextLine();
                                        Office office = new Office(jobType, ownerOfOffice);

                                        int posOfOffice;
                                        int lenOfOffice;
                                        int heightOfOffice;
                                        String sideOfStreet;
                                        boolean flagOffice = true;
                                        while (flagOffice){
                                            try {
                                                System.out.println("Enter office position, length, height and side accordingly: (ex. 3 5 6 left) ");
                                                posOfOffice = scanner.nextInt();
                                                lenOfOffice = scanner.nextInt();
                                                heightOfOffice = scanner.nextInt();
                                                scanner.nextLine();
                                                sideOfStreet = scanner.nextLine();
                                                if(!(sideOfStreet.equals("left")) && !(sideOfStreet.equals("LEFT"))&& !(sideOfStreet.equals("right"))&& !(sideOfStreet.equals("RIGHT"))){
                                                    System.out.println("left, LEFT, right, RIGHT are acceptable words. Try again!");
                                                }else{
                                                    sideOfStreet = sideOfStreet.toLowerCase(Locale.ROOT);
                                                    if (posOfOffice<0 || posOfOffice > street.getLengthOfStreet() || posOfOffice+lenOfOffice > street.getLengthOfStreet()){
                                                        System.out.println("Out of street bound. Try again!");
                                                    }else{
                                                        office.setPositionLengthHeight(posOfOffice, lenOfOffice, heightOfOffice);
                                                        if (user.addBuilding(street, office, sideOfStreet)){
                                                            flagOffice = false;
                                                        }
                                                        else{
                                                            System.out.println("That position is not on empty land area. Try again!");
                                                        }
                                                    }
                                                }

                                            } catch (InputMismatchException e){
                                                System.out.println("Input mismatch exception occured. Try again!");
                                                scanner.next();
                                            }
                                        }
                                        // Playground Creation
                                    }else if(buildingTypeOption == 4){


                                        Playground playground = new Playground();
                                        int posOfPlayground;
                                        int lenOfPlayground;
                                        String sideOfStreet;
                                        boolean flagPlayground = true;
                                        while (flagPlayground){
                                            try {
                                                System.out.println("Enter playground position, length and side accordingly: (ex. 3 6 left) ");
                                                posOfPlayground = scanner.nextInt();
                                                lenOfPlayground = scanner.nextInt();
                                                scanner.nextLine();
                                                sideOfStreet = scanner.nextLine();
                                                if(!(sideOfStreet.equals("left")) && !(sideOfStreet.equals("LEFT"))&& !(sideOfStreet.equals("right"))&& !(sideOfStreet.equals("RIGHT"))){
                                                    System.out.println("left, LEFT, right, RIGHT are acceptable words. Try again!");
                                                }else{
                                                    sideOfStreet = sideOfStreet.toLowerCase(Locale.ROOT);
                                                    if (posOfPlayground<0 || posOfPlayground > street.getLengthOfStreet() || posOfPlayground+lenOfPlayground > street.getLengthOfStreet()){
                                                        System.out.println("Out of street bound. Try again!");
                                                    }else{
                                                        playground.setPositionLengthHeight(posOfPlayground, lenOfPlayground, playground.getHeight());
                                                        if (user.addBuilding(street, playground, sideOfStreet)){
                                                            flagPlayground = false;
                                                        }
                                                        else{
                                                            System.out.println("That position is not on empty land area. Try again!");
                                                        }
                                                    }
                                                }

                                            } catch (InputMismatchException e){
                                                System.out.println("Input mismatch exception occured. Try again!");
                                                scanner.next();
                                            }
                                        }
                                    }else if(buildingTypeOption == 5){
                                        System.out.println("Going back..");
                                        buildingTypeFlag = false;
                                    }else {
                                        System.out.println("Invalid Input");
                                    }
                                }
                                break;
                                // Remove Building
                            case 2:

                                int posOfRemoveBuilding=0;
                                boolean flagRemoving = true;
                                String sideTemp = "";
                                while (flagRemoving){
                                    try {
                                        System.out.println("Enter position and side of building you want to remove: (ex. 3 left)");
                                        // it gets position of building from user
                                        posOfRemoveBuilding = scanner.nextInt();
                                        scanner.nextLine();
                                        sideTemp = scanner.nextLine();
                                        flagRemoving = false;
                                    }catch (Exception e){
                                        System.out.println("Input mismatch exception! try again...");
                                        scanner.next();
                                    }
                                }

                                ArrayList<Structure> tempLeft = street.getLeftSideBuildings();
                                ArrayList<Structure> tempRight = street.getRightSideBuildings();

                                // it looks all indexes and if a structure positionbegin property is equals to opsition which is got from user
                                // it calls deleteBuilding func and removes the structure.
                                // Left side check
                                if(sideTemp.equals("left")){
                                    for (int i = 0; i < tempLeft.size(); i++) {
                                        Structure temp = (Structure) tempLeft.get(i);
                                        if (temp.getPositionBegin() == posOfRemoveBuilding){
                                            street.deleteBuilding("left", temp);
                                            System.out.println("Structure Removed!");
                                            break;
                                        }
                                    }
                                    // right side check
                                } else if(sideTemp.equals("right")){
                                    for (int i = 0; i < tempRight.size(); i++) {
                                        Structure temp = (Structure) tempRight.get(i);
                                        if (temp.getPositionBegin() == posOfRemoveBuilding){
                                            street.deleteBuilding("right", temp);
                                            System.out.println("Structure Removed!");
                                            break;
                                        }
                                    }
                                } else{
                                    System.out.println("There is no building at that location.");
                                }
                                break;
                            case 3:
                                System.out.println("going to mode selection!");
                                editFlag = false;
                                break;
                            default:
                                System.out.println("Invalid Input");
                        }
                    }
                    break;

                    // View mode
                case 2:
                    System.out.println("********* View Mode *************");
                    boolean viewFlag = true;
                    if (street.getLengthOfStreet() == 0){
                        System.out.println("You should create a street in edit mode. Menu loading...");
                        break;
                    }

                    while (viewFlag){
                        int viewOption=0;
                        System.out.println("Press 1 to display the total remaining length of lands on the street.");
                        System.out.println("Press 2 to display the list of buildings on the street.");
                        System.out.println("Press 3 to display the number and ratio of lenth of playgrounds in the street.");
                        System.out.println("Press 4 to display the total length of street occupied by the markets, houses or offices.");
                        System.out.println("Press 5 to display the skyline silhouette of the street.");
                        System.out.println("Press 6 to focus any buildings.");
                        System.out.println("Press 7 to Exit.");

                        try {
                            viewOption = scanner.nextInt();
                        }
                        catch (InputMismatchException e){
                            System.out.println("Input mismatch exception!");
                            scanner.next();
                        }
                        switch (viewOption){
                            case 1:
                                user.totalRemainingLengthOfLands(street);
                                break;
                            case 2:
                                user.listBuildingsOnTheStreet(street);
                                break;
                            case 3:
                                user.numberAndRatioOfPlaygrounds(street);
                                break;
                            case 4:
                                user.totalLengthOfBuildings(street);
                                break;
                            case 5:
                                user.skylineSilhouette(street);
                                break;
                            case 6:

                                int tempPos;
                                String sideOfStreet;
                                boolean flagFocus = true;
                                while (flagFocus){
                                    try {
                                        System.out.println("Enter position and side to focus building.");
                                        tempPos = scanner.nextInt();
                                        scanner.nextLine();
                                        sideOfStreet = scanner.nextLine();
                                        if(!(sideOfStreet.equals("left")) && !(sideOfStreet.equals("LEFT"))&& !(sideOfStreet.equals("right"))&& !(sideOfStreet.equals("RIGHT"))){
                                            System.out.println("left, LEFT, right, RIGHT are acceptable words. Try again!");
                                        }
                                        else{
                                            sideOfStreet = sideOfStreet.toLowerCase(Locale.ROOT);
                                            user.focus(street, tempPos, sideOfStreet);
                                            flagFocus = false;
                                        }

                                    } catch (InputMismatchException e){
                                        System.out.println("Input mismatch exception occured. Try again!");
                                        scanner.next();
                                    }
                                }

                                break;
                            case 7:
                                System.out.println("going to mode selection!");
                                viewFlag = false;
                                break;
                            default:
                                System.out.println("Invalid Input");
                        }
                    }
                    break;
                case 3:
                    System.out.println("Goodbye!");
                    flag = false;
                    break;
                default:
                    System.out.println("Invalid Input");
            }
        }
    }
    // Driver code to show required actions automaticly.
    public static void driver(){


        Street street1 = new Street();
        System.out.println("A street is created.");
        User user1 = new User();
        System.out.println("User object created.");
        user1.setLengthOfStreet(street1, 15);
        System.out.println("street length is set to 15 by user.");


        Playground playground1 = new Playground(3, 12);
        System.out.println("A playground is created. Length : 3 and position: 12");

        Market market1 = new Market("9:00AM", "7:00PM");
        System.out.println("Market object is created. Opening time: 9:00AM and closing time: 7:00PM");

        Market market2 = new Market("8:30AM", "5:30PM");
        System.out.println("Market object is created. Opening time: 8:30AM and closing time: 5:30PM");

        Office office1 = new Office("software", "cem bulut");
        System.out.println("Office object is created. Job type: software, owner: cem bulut");

        Office office2 = new Office("insurance", "kaya gurger");
        System.out.println("Office object is created. Job type: insurance, owner: kaya gurger");

        Office office3 = new Office("fortune teller", "sadiye seker");
        System.out.println("Office object is created. Job type: fortune teller, owner: sadiye seker");

        House house1 = new House(3, "blue", "veli guder");
        System.out.println("House object is created. num. of rooms: 3, color: blue, owner: veli guder");

        House house2 = new House(2, "black", "arzu sahin");
        System.out.println("House object is created. num. of rooms: 2, color: black, owner: arzu sahin");

        House house3 = new House(5, "white", "nesin beyaz");
        System.out.println("House object is created. num. of rooms: 5, color: white, owner: nesin beyaz");

        House house4 = new House(1, "grey", "guru buru");
        System.out.println("House object is created. num. of rooms: 1, color: grey, owner: guru buru");
        System.out.println();

        playground1.setPositionLengthHeight(6, 6, 10);
        System.out.println("Playground1 position(6), length(6) and height(10) are set.");
        market1.setPositionLengthHeight(2, 5, 4);
        System.out.println("Market1 position(2), length(5) and height(4) are set.");
        market2.setPositionLengthHeight(5, 5, 8);
        System.out.println("Market2 position(5), length(5) and height(8) are set.");
        house1.setPositionLengthHeight(2, 4,6);
        System.out.println("House1 position(2), length(4) and height(6) are set.");
        office1.setPositionLengthHeight(1, 4, 13);
        System.out.println("Office1 position(1), length(4) and height(13) are set.");
        office2.setPositionLengthHeight(2,3,4);
        System.out.println("Office2 position(2), length(3) and height(4) are set.");
        System.out.println();


            user1.addBuilding(street1, market1, "left");
               user1.addBuilding(street1, market2, "right");
    user1.addBuilding(street1, office1, "left");
     user1.addBuilding(street1, house1, "right");
        user1.skylineSilhouette(street1);
        System.out.println("Skyline silhouette is drawn.");
        System.out.println();


        user1.listBuildingsOnTheStreet(street1);
        user1.numberAndRatioOfPlaygrounds(street1);
        user1.totalLengthOfBuildings(street1);
        user1.totalRemainingLengthOfLands(street1);

        //market1.focus();
        user1.focus(street1, 2, "left");
        System.out.println("Market1 focus method is called.");
        //market2.focus();
        user1.focus(street1, 5, "right");
        System.out.println("Market2 focus method is called.");
        //office1.focus();
        user1.focus(street1, 1, "left");
        System.out.println("Office1 focus method is called. It is empty because Office1 cannot be set to street because of not empty land");
        //house1.focus();
        user1.focus(street1, 2, "right");
        System.out.println("House1 focus method is called. It is empty because House1 cannot be set to street because of not empty land");

        user1.deleteBuilding(street1, "right", market2);
        System.out.println("Market2 is deleted from right side.");

        user1.skylineSilhouette(street1);
        System.out.println("Skyline silhouette is drawn.");
    }

}
















