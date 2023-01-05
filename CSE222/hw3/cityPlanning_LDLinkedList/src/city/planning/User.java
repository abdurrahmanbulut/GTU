package city.planning;


public class User implements IUser, IEditMode, IViewMode{


    @Override
    public void setPositionLengthHeightOfBuilding(Structure structure, int position, int len, int height) {
        structure.setPositionLengthHeight(position, len, height);
    }

    @Override
    public void setLengthOfStreet(Street street, int lengthOfStreet) {
        street.setLengthOfStreet(lengthOfStreet);
    }

    @Override
    public boolean addBuilding(Street street, Structure structure, String side) {
        return street.putBuilding(structure, side);
    }

    @Override
    public void deleteBuilding(Street street, String side, Structure structure) {
        street.deleteBuilding(side, structure);
    }

    // display the total remaining length of lands on the street.
    @Override
    public void totalRemainingLengthOfLands(Street street) {
        LDLinkedList<Structure> tempLeft = street.getLeftSideBuildings();
        LDLinkedList<Structure> tempRight = street.getRightSideBuildings();

        // there are two side of street, so all space is street len * 2
        // it substracts all used lands from remainlen variable.
        int remainLen = street.getLengthOfStreet()*2;
        int usedLandLength=0;
        for (int i = 0; i < tempLeft.size(); i++) {
            Structure temp = (Structure) tempLeft.get(i);
            usedLandLength += temp.getLength();
        }
        for (int i = 0; i < tempRight.size(); i++) {
            Structure temp = (Structure) tempRight.get(i);
            usedLandLength += temp.getLength();
        }
        remainLen -= usedLandLength;

        System.out.println("Total remaining length of lands on the street: " + remainLen);
    }

    // display the list of buildings on the street.
    // it looks at all indexes of two sides and counts building types.
    @Override
    public void listBuildingsOnTheStreet(Street street) {
        int marketCount = 0, houseCount = 0, officeCount = 0;

        LDLinkedList<Structure> tempLeft = street.getLeftSideBuildings();
        LDLinkedList<Structure> tempRight = street.getRightSideBuildings();

        // Left side buildings
        for (int i = 0; i < tempLeft.size(); i++) {
            Structure temp = (Structure) tempLeft.get(i);
            if (temp.getClass().getSimpleName().equals("Market"))
                marketCount++;
            else if (temp.getClass().getSimpleName().equals("House"))
                houseCount++;
            else if (temp.getClass().getSimpleName().equals("Office"))
                officeCount++;

        }
        // Right side buildings
        for (int i = 0; i < tempRight.size(); i++) {
            Structure temp = (Structure) tempRight.get(i);
            if (temp.getClass().getSimpleName().equals("Market"))
                marketCount++;
            else if (temp.getClass().getSimpleName().equals("House"))
                houseCount++;
            else if (temp.getClass().getSimpleName().equals("Office"))
                officeCount++;
        }
        System.out.println("Building lists: ");
        System.out.println("Market: " + marketCount + "\n" + "House: " + houseCount + "\n" + "Office: " + officeCount + "\n");
    }

    // display the number and ratio of lenth of playgrounds in the street.
    @Override
    public void numberAndRatioOfPlaygrounds(Street street) {
        int playgroundCount=0, playgroundLen=0, streetLen=0;

        LDLinkedList<Structure> tempLeft = street.getLeftSideBuildings();
        LDLinkedList<Structure> tempRight = street.getRightSideBuildings();

        // left side playground counts and length
        for (int i = 0; i < tempLeft.size(); i++) {
            Structure temp = (Structure) tempLeft.get(i);
            if (temp.getClass().getSimpleName().equals("Playground")){
                playgroundCount++;
                playgroundLen += temp.length;
            }
            streetLen += temp.length;

        }
        // Right side playground counts and length
        for (int i = 0; i < tempRight.size(); i++) {
            Structure temp = (Structure) tempRight.get(i);
            if (temp.getClass().getSimpleName().equals("Playground")){
                playgroundCount++;
                playgroundLen += temp.length;
            }
            streetLen += temp.length;
        }
        System.out.println("Playground number and ratio of playgrounds len to street len:");
        System.out.println("Playground: " + playgroundCount);

        // Exception handling for division.
        try {
            System.out.println("Playground / Street length ratio: " + (double)playgroundCount / streetLen);
        }
        catch (ArithmeticException e){
            System.out.println(e);
        }
    }

    // calculate the total length of street occupied by the markets, houses or offices.
    @Override
    public void totalLengthOfBuildings(Street street) {
        int houseLen=0, officeLen=0, marketLen=0;

        LDLinkedList<Structure> tempLeft = street.getLeftSideBuildings();
        LDLinkedList<Structure> tempRight = street.getRightSideBuildings();

        // left side buildings length
        for (int i = 0; i < tempLeft.size(); i++) {
            Structure temp = (Structure) tempLeft.get(i);
            if (temp.getClass().getSimpleName().equals("Market")){
                marketLen += temp.length;
            }
            else if (temp.getClass().getSimpleName().equals("Office")){
                officeLen += temp.length;
            }
            else if (temp.getClass().getSimpleName().equals("House")){
                houseLen += temp.length;
            }
        }

        //right side building length
        for (int i = 0; i < tempRight.size(); i++) {
            Structure temp = (Structure) tempRight.get(i);
            if (temp.getClass().getSimpleName().equals("Market")){
                marketLen += temp.length;
            }
            else if (temp.getClass().getSimpleName().equals("Office")){
                officeLen += temp.length;
            }
            else if (temp.getClass().getSimpleName().equals("House")){
                houseLen += temp.length;
            }
        }
        System.out.println("Total length of buildings: ");
        System.out.println("Market: " + marketLen + "\n" + "House: " + houseLen + "\n" + "Office: " + officeLen + "\n");

    }

    // display the skyline silhouette of the street
    // this func. gets highest values from two sides with comparison.
    // it stores all highest height values on an array.
    // then it prints from starting point(index 0).
    @Override
    public void skylineSilhouette(Street street) {
        LDLinkedList<Structure> tempLeft = street.getLeftSideBuildings();
        LDLinkedList<Structure> tempRight = street.getRightSideBuildings();

        int longestBuildingHeight = 0, streetLen=0;
        streetLen = street.getLengthOfStreet();


        int[] heightValues = new int[streetLen];

        // highest building height is founded.
        for (int i = 0; i < tempRight.size(); i++) {
            Structure temp = (Structure) tempRight.get(i);
            if (longestBuildingHeight < temp.getHeight()) longestBuildingHeight = temp.getHeight();
        }
        for (int i = 0; i < tempLeft.size(); i++) {
            Structure temp = (Structure) tempLeft.get(i);
            if (longestBuildingHeight < temp.getHeight()) longestBuildingHeight = temp.getHeight();
        }

        // in this part, all cross buildings are compared and
        int highestBuild;
        for (int i = 0; i < streetLen; i++) {
            highestBuild=0;
            for (int j = 0; j < tempLeft.size(); j++) {
                Structure temp = (Structure) tempLeft.get(j);
                try{
                    if (i <= temp.getPositionEnd() && i > temp.getPositionBegin()){
                        if (highestBuild < temp.getHeight())
                            highestBuild = temp.getHeight();
                    }
                }catch (NullPointerException e){
                    // getPositionEnd may give an error. Because "temp" may be null.
                }

            }
            for (int j = 0; j < tempRight.size(); j++) {
                Structure temp = (Structure) tempRight.get(j);
                try{
                    if (i <= temp.getPositionEnd() && i > temp.getPositionBegin()){
                        if (highestBuild < temp.getHeight())
                            highestBuild = temp.getHeight();
                    }
                }
                catch (NullPointerException e){
                    // getPositionEnd may give an error. Because "temp" may be null. So, this part can be empty.
                }

            }
            heightValues[i] = highestBuild;
        }
        System.out.println();
        // to fixed height of street. If max height of building is higher than 15, it will skip this loop.
        for (int i = 0; i < 15-longestBuildingHeight; i++) {
            for (int j = 1; j < heightValues.length; j++) {
                System.out.print(" ");
            }
            System.out.println();
        }
        // It starts to draw by looking at the highest building height.
        for (int i = longestBuildingHeight; i > 0; i--) {
            for (int j = 1; j < heightValues.length; j++) {

                if (i == heightValues[j]){

                    System.out.print("*");
                    heightValues[j]--;
                }
                else {
                    System.out.print(" ");
                }
                System.out.print("  ");
            }
            System.out.print("\n");
            
        }
        for (int i = 0; i < heightValues.length; i++) {

            System.out.format("%-2d", i);
            System.out.print(" ");
        }
        System.out.println();
    }
    // focus method for user
    // user can call focus methods.
    public void focus(Street street, int position, String side){
        LDLinkedList<Structure> tempLeft = street.getLeftSideBuildings();
        LDLinkedList<Structure> tempRight = street.getRightSideBuildings();

        if(side.equals("left")){
            for (int i = 0; i < tempLeft.size() ; i++) {
                Structure temp = (Structure) tempLeft.get(i);
                if(temp.getPositionBegin() == position){
                    if (temp.getClass().getSimpleName().equals("Market")){
                        Market tempM = (Market) temp;
                        tempM.focus();
                    } else if (temp.getClass().getSimpleName().equals("Office")){
                        Office tempM = (Office) temp;
                        tempM.focus();
                    } else if (temp.getClass().getSimpleName().equals("House")){
                        House tempM = (House) temp;
                        tempM.focus();
                    } else if (temp.getClass().getSimpleName().equals("Playground")){
                        Playground tempM = (Playground) temp;
                        tempM.focus();
                    }
                    return;
                }
            }
        }else if(side.equals("right")){
            for (int i = 0; i < tempRight.size() ; i++) {
                Structure temp = (Structure) tempRight.get(i);
                if(temp.getPositionBegin() == position){
                    if (temp.getClass().getSimpleName().equals("Market")){
                        Market tempM = (Market) temp;
                        tempM.focus();
                    } else if (temp.getClass().getSimpleName().equals("Office")){
                        Office tempM = (Office) temp;
                        tempM.focus();
                    } else if (temp.getClass().getSimpleName().equals("House")){
                        House tempM = (House) temp;
                        tempM.focus();
                    } else if (temp.getClass().getSimpleName().equals("Playground")){
                        Playground tempM = (Playground) temp;
                        tempM.focus();
                    }
                    return;
                }
            }
        }
        else{
            return;
        }


    }
}

