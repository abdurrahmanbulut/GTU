package city.planning;

import java.util.Objects;

public class Street {

    private int lengthOfStreet;
    MyArray<Structure> leftSideBuildings;
    MyArray<Structure> rightSideBuildings;

    // getters for buildings array.
    public MyArray<Structure> getLeftSideBuildings() {
        return leftSideBuildings;
    }
    public MyArray<Structure> getRightSideBuildings() {
        return rightSideBuildings;
    }

    public Street() {
        leftSideBuildings = new MyArray<>();
        rightSideBuildings = new MyArray<>();
    }

    public int getLengthOfStreet() {
        return lengthOfStreet;
    }

    public void setLengthOfStreet(int lengthOfStreet){
        this.lengthOfStreet = lengthOfStreet;
    }

    // this func. gets a structure(market, office, house, playground) and a side(left, right).
    // it puts the structure to created arrays. if there are enough space on street, it puts the structure and returns true.
    // Otherwise it returns false and does not put structure to street array.
    public boolean putBuilding(Structure structure, String side){
        boolean flag = true;
        if (Objects.equals(side, "left")){
            for (int i = 0; i < leftSideBuildings.getSize(); i++) {
                Structure temp = (Structure) leftSideBuildings.getArrayElement(i);

                // it checks the position of new building is empty or not.
                if ((structure.positionBegin >= temp.positionBegin && structure.positionEnd <= temp.positionEnd) ||
                        (structure.positionBegin <= temp.positionBegin && structure.positionEnd >= temp.positionEnd) ||
                        (structure.positionBegin <= temp.positionEnd && structure.positionEnd >= temp.positionBegin) ||
                        (structure.positionBegin <= temp.positionEnd && structure.positionEnd >= temp.positionEnd)){

                    flag = false;
                    break;
                }
            }
            //if there are enough land, it adds structure to street and returns true.
            if (flag){

                leftSideBuildings.add(structure);
                System.out.println("Structure is added to street left side.");
                return true;
            }
            else{
                System.out.println("Structure is not added. There is no enough space in the street");
                return false;
            }
        }
        else if (Objects.equals(side, "right")){
            for (int i = 0; i < rightSideBuildings.getSize(); i++) {
                Structure temp = (Structure) rightSideBuildings.getArrayElement(i);

                // it checks the position of new building is empty or not.
                if ((structure.positionBegin >= temp.positionBegin && structure.positionEnd <= temp.positionEnd) ||
                        (structure.positionBegin <= temp.positionBegin && structure.positionEnd >= temp.positionEnd) ||
                        (structure.positionBegin <= temp.positionEnd && structure.positionEnd >= temp.positionBegin) ||
                        (structure.positionBegin <= temp.positionEnd && structure.positionEnd >= temp.positionEnd)){

                    flag = false;
                    break;
                }
            }
            if (flag){

                rightSideBuildings.add(structure);
                System.out.println("Structure is added to street right side.");
                return true;
            }
            else{
                System.out.println("Structure is not added. There is no enough space in the street");
                return false;
            }
        }
        else{
            return false;
        }
    }
    // this func gets an object ref. and a side(left or right). It uses delete func of MyArray container class.
    // if structure is there, it will be removed.
    public void deleteBuilding(String side, Structure structure){
        if (Objects.equals(side, "left")){
            leftSideBuildings.delete(structure);
        }
        else if (Objects.equals(side, "right")){
            rightSideBuildings.delete(structure);
        }
    }
}
