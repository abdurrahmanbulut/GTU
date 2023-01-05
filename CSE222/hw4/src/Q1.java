public class Q1 {

    public static int stringSearch(String str, String bigStr, int ith, int index) {
        if (bigStr.length() == 0 || str.length() == 0)
            return -1;

        String[] temp = bigStr.split(str, 2);


        if (temp.length >= 2) {
            ith--;
            if (ith >= 1){
                index += str.length();
                index += temp[0].length();
            }

            if (ith == 0) {
                index += temp[0].length();
                return index;
            }
        }
        if (temp.length == 1) return -1;

        return stringSearch(str, String.valueOf(temp[1]), ith, index);
    }

}
