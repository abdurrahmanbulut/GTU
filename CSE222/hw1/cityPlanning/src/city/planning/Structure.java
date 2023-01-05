package city.planning;

public abstract class Structure {
    protected int height;
    protected int length;
    protected int positionBegin;
    protected int positionEnd;

    public Structure() {

    }

    public Structure(int height, int length, int position) {
        this.height = height;
        this.length = length;
        this.positionBegin = position;
        this.positionEnd = position + length;
    }
    // it gets just position. y looking at position, it sets position end and begin.
    public void setPositionLengthHeight(int position, int len, int height){
        this.positionBegin = position;
        this.length = len;
        this.height = height;
        this.positionEnd = position+len;
    }

    public int getPositionBegin() {
        return positionBegin;
    }

    public int getPositionEnd() {
        return positionEnd;
    }


    public int getHeight() {
        return height;
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }
}
