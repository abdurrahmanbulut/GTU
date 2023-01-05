import java.util.Random;

public class Driver {
    public static void main(String[] args) {

        System.out.println("************");
        System.out.println("** PART 1 **");
        System.out.println("************");
        long startTime, elapsedTime;

        startTime = System.nanoTime();
        System.out.println();
        System.out.println("---------  Binary Search Tree Hash Map Test  ---------");
        HashtableChain<Integer, String> newHt = new HashtableChain<>();
        System.out.println("hash Map Created....");
        newHt.put(5, "test1");
        newHt.put(3, "test2");
        newHt.put(15, "test3");
        newHt.put(12, "test4");
        newHt.put(2, "test5");
        newHt.put(277, "test6");
        newHt.put(53, "test7");
        newHt.put(232, "test7");
        newHt.put(1, "test7");
        newHt.put(65, "test7");
        newHt.put(1010, "test7");
        newHt.put(34, "test6");
        newHt.put(78, "test7");
        newHt.put(35888, "test7");
        System.out.println("Added some Entries...");
        System.out.println("Hash Map: " + newHt.toString());
        newHt.remove(2);
        System.out.println("Remove key 2....");
        System.out.println("Hash Map: " + newHt.toString());
        newHt.put(2, "test5");
        System.out.println("Key 2 added again...");
        System.out.println("Hash Map: " + newHt.toString());
        System.out.println("Test to remove non-existent");
        System.out.println("Remove key 25 : " + newHt.remove(25));
        System.out.println("Hash Map: " + newHt.toString());
        System.out.println("Some getter tests...");
        System.out.println("Value of key 12:" + newHt.get(12));
        System.out.println("Value of key 3:" + newHt.get(3));
        System.out.println("Value of key 27:" + newHt.get(277));
        System.out.println("Value of key 22:" + newHt.get(22));
        System.out.println("Is empty? : " + newHt.isEmpty());

        elapsedTime = System.nanoTime() - startTime;
        System.out.println("Total execution time : " + elapsedTime / 1000000);


        int[] testVal = new int[]{3, 12, 13, 25, 23, 51};
        System.out.println("\n\n");
        System.out.println("************");
        System.out.println("** PART 2 **");
        System.out.println("************");

        // initializing KWHashMap objects
        KWHashMap<Integer, String> myChain = new HashtableChain<>();
        KWHashMap<Integer, String> myCoalesced = new HashtableCoalesced<>();

        // time holders
        long timeChain;
        long timeTree;
        long timeCoalesced;

        // bound for rand numbers
        final int bound = 1000;

        // random numbers
        int[] small = new int[5];
        int[] medium = new int[100];
        int[] large = new int[2000];

        // random seed
        Random random = new Random();

        // filling random number arrays
        for (int i = 0; i < small.length; i++)
            small[i] = random.nextInt(bound);
        for (int i = 0; i < medium.length; i++)
            medium[i] = random.nextInt(bound);
        for (int i = 0; i < large.length; i++)
            large[i] = random.nextInt(bound);

        // getting existing / non existing elements
        int[] existingSmall = new int[5];
        int[] existingMedium = new int[10];
        int[] existingLarge = new int[100];
        int nonExisting = bound + 99;

        // filling existing elements
        for (int i = 0; i < existingSmall.length; i++)
            existingSmall[i] = testVal[i];
        for (int i = 0; i < existingMedium.length; i++)
            existingMedium[i] = medium[i];
        for (int i = 0; i < existingLarge.length; i++)
            existingLarge[i] = large[i];


        System.out.println("\nInserting 5 elements into hashTables");
        System.out.println("------------------------------------");

        System.out.println("\nInserting...");

        timeChain = System.nanoTime();
        for (int j : testVal) myChain.put(j, Integer.toString(j - 10));
        timeChain = System.nanoTime() - timeChain;

        timeCoalesced = System.nanoTime();
        for (int j : testVal) myCoalesced.put(j, Integer.toString(j - 10));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        // printing out hashmaps
        System.out.println("\n-- HashtableChain --");
        System.out.println(myChain);


        System.out.println("\n-- HashtableCoalesced -- \n");
        System.out.println(myCoalesced);

        System.out.println("\nTime collapsed while inserting 5 elements into HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while inserting 5 elements into HashtableCoalesced object : " + timeCoalesced + " nanosecond");

        System.out.println("\nGetting 2 elements which exist in hashTables");
        System.out.println("---------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        for (int k : existingSmall) System.out.println("HashtableChain     -> Key: " + k + " Value: " + myChain.get(k));
        timeChain = System.nanoTime() - timeChain;


        System.out.println();

        timeCoalesced = System.nanoTime();
        for (int k : existingSmall)
            System.out.println("HashtableCoalesced -> Key: " + k + " Value: " + myCoalesced.get(k));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting 2 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting 2 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nGetting one element which not exist in hashTables");
        System.out.println("------------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        System.out.println("HashtableChain     -> Key: " + nonExisting + " Value: " + myChain.get(nonExisting));
        timeChain = System.nanoTime() - timeChain;

        System.out.println();

        timeCoalesced = System.nanoTime();
        System.out.println("HashtableCoalesced -> Key: " + nonExisting + " Value: " + myCoalesced.get(nonExisting));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting one element which not exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving 2 element that exist in hashTables");
        System.out.println("-------------------------------------------");

        System.out.println("\nRemoving... (" + existingSmall[0] + ", " + existingSmall[1] + ")");

        int sizeBeforeRem_chain, sizeAfterRem_chain;
        int sizeBeforeRem_tree, sizeAfterRem_tree;
        int sizeBeforeRem_coalesced, sizeAfterRem_coalesced;

        System.out.println();

        sizeBeforeRem_chain = myChain.size();
        timeChain = System.nanoTime();
        for (int i = 0; i < existingSmall.length; i++)
            myChain.remove(existingSmall[i]);
        timeChain = System.nanoTime() - timeChain;
        sizeAfterRem_chain = myChain.size();


        sizeBeforeRem_coalesced = myCoalesced.size();
        timeCoalesced = System.nanoTime();
        for (int i = 0; i < existingSmall.length; i++)
            myCoalesced.remove(existingSmall[i]);
        timeCoalesced = System.nanoTime() - timeCoalesced;
        sizeAfterRem_coalesced = myCoalesced.size();

        System.out.println("(HashtableChain)     Size before removing : " + sizeBeforeRem_chain + " Size after removing : " + sizeAfterRem_chain);
        System.out.println("(HashtableCoalesced) Size before removing : " + sizeBeforeRem_coalesced + " Size after removing : " + sizeAfterRem_coalesced);

        System.out.println("\nTime collapsed while removing 2 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while removing 2 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        // MEDIUM DATA

        // initializing KWHashMap objects
        myChain = new HashtableChain<>();
        myCoalesced = new HashtableCoalesced<>();


        System.out.println("\nInserting 100 elements into hashTables");
        System.out.println("------------------------------------");

        System.out.println("\nInserting...");

        timeChain = System.nanoTime();
        for (int j : medium) myChain.put(j, Integer.toString(j - 10));
        timeChain = System.nanoTime() - timeChain;

        timeCoalesced = System.nanoTime();
        for (int j : medium) myCoalesced.put(j, Integer.toString(j - 10));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while inserting 100 elements into HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while inserting 100 elements into HashtableCoalesced object : " + timeCoalesced + " nanosecond");

        System.out.println("\nGetting 10 elements which exist in hashTables");
        System.out.println("----------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        for (int k : existingMedium) myChain.get(k);
        timeChain = System.nanoTime() - timeChain;

        timeCoalesced = System.nanoTime();
        for (int k : existingMedium) myCoalesced.get(k);
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting 2 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting 2 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nGetting one element which not exist in hashTables");
        System.out.println("------------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        System.out.println("HashtableChain     -> Key: " + nonExisting + " Value: " + myChain.get(nonExisting));
        timeChain = System.nanoTime() - timeChain;

        System.out.println();

        timeCoalesced = System.nanoTime();
        System.out.println("HashtableCoalesced -> Key: " + nonExisting + " Value: " + myCoalesced.get(nonExisting));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting one element which not exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving 10 element that exist in hashTables");
        System.out.println("--------------------------------------------");

        System.out.println("\nRemoving...");

        System.out.println();

        sizeBeforeRem_chain = myChain.size();
        timeChain = System.nanoTime();
        for (int i = 0; i < existingMedium.length; i++)
            myChain.remove(existingMedium[i]);
        timeChain = System.nanoTime() - timeChain;
        sizeAfterRem_chain = myChain.size();

        sizeBeforeRem_coalesced = myCoalesced.size();
        timeCoalesced = System.nanoTime();
        for (int i = 0; i < existingMedium.length; i++)
            myCoalesced.remove(existingMedium[i]);
        timeCoalesced = System.nanoTime() - timeCoalesced;
        sizeAfterRem_coalesced = myCoalesced.size();

        System.out.println("(HashtableChain)     Size before removing : " + sizeBeforeRem_chain + " Size after removing : " + sizeAfterRem_chain);
        System.out.println("(HashtableCoalesced) Size before removing : " + sizeBeforeRem_coalesced + " Size after removing : " + sizeAfterRem_coalesced);

        System.out.println("\nTime collapsed while removing 10 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while removing 10 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        // LARGE DATA

        // initializing KWHashMap objects
        myChain = new HashtableChain<>();
        myCoalesced = new HashtableCoalesced<>();


        System.out.println("\nInserting 2000 elements into hashTables");
        System.out.println("------------------------------------");

        System.out.println("\nInserting...");

        timeChain = System.nanoTime();
        for (int j : large) myChain.put(j, Integer.toString(j - 10));
        timeChain = System.nanoTime() - timeChain;

        timeCoalesced = System.nanoTime();
        for (int j : large) myCoalesced.put(j, Integer.toString(j - 10));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while inserting 2000 elements into HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while inserting 2000 elements into HashtableCoalesced object : " + timeCoalesced + " nanosecond");

        System.out.println("\nGetting 100 elements which exist in hashTables");
        System.out.println("------------*----------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        for (int k : existingLarge) myChain.get(k);
        timeChain = System.nanoTime() - timeChain;

        timeCoalesced = System.nanoTime();
        for (int k : existingLarge) myCoalesced.get(k);
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting 100 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting 100 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nGetting one element which not exist in hashTables");
        System.out.println("------------------------------------------------");

        System.out.println("\nGetting...\n");

        timeChain = System.nanoTime();
        System.out.println("HashtableChain     -> Key: " + nonExisting + " Value: " + myChain.get(nonExisting));
        timeChain = System.nanoTime() - timeChain;

        System.out.println();

        timeCoalesced = System.nanoTime();
        System.out.println("HashtableCoalesced -> Key: " + nonExisting + " Value: " + myCoalesced.get(nonExisting));
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("\nTime collapsed while getting one element which not exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while getting one element which not exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving 500 element that exist in hashTables");
        System.out.println("---------------------------------------------");

        System.out.println("\nRemoving...");

        System.out.println();

        timeChain = System.nanoTime();
        for (int i = 0; i < existingLarge.length; i++) {
            try {
                myChain.remove(existingLarge[i]);
            } catch (NullPointerException ignore) {
            }
        }
        timeChain = System.nanoTime() - timeChain;

        timeCoalesced = System.nanoTime();
        for (int i = 0; i < existingLarge.length; i++)
            try {
                myCoalesced.remove(existingLarge[i]);
            } catch (NullPointerException ignore) {
            }
        timeCoalesced = System.nanoTime() - timeCoalesced;

        System.out.println("Time collapsed while removing 100 elements that exist in HashtableChain object     : " + timeChain + " nanosecond");
        System.out.println("Time collapsed while removing 100 elements that exist in HashtableCoalesced object : " + timeCoalesced + " nanosecond");


        System.out.println("\nRemoving one element that doesn't exist in hashTables");
        System.out.println("----------------------------------------------------");
        try {
            myChain.remove(nonExisting);
        } catch (NullPointerException n) {
            System.err.println("The item you are trying to remove doesn't exist in the hashmap");
        }
        try {
            myCoalesced.remove(nonExisting);
        } catch (NullPointerException n) {
            System.err.println("The item you are trying to remove doesn't exist in the hashmap");
        }

        /* Search Algorithms Start */
        System.out.println();
        System.out.println("************");
        System.out.println("** PART 3 **");
        System.out.println("************");
        Random rand = new Random();
        long startQuick, startMerge, startNew, endQuick, endMerge, endNew, totalQuick = 0, totalMerge = 0, totalNew = 0;

        System.out.println("---------  Search Algorithms Tests  ---------");

        System.out.println("Integer array with size 100 is created.");
        System.out.println();


        for (int k = 100; k <= 10000; k *= 10) {
            Integer[] test100 = new Integer[k];

            System.out.println("1000 arrays with size : " + k + " working");
            for (int j = 0; j < 1000; j++) {
                for (int i = 0; i < test100.length; i++) {
                    test100[i] = rand.nextInt();
                }
                startQuick = System.nanoTime();
                QuickSort.sort(test100);
                endQuick = System.nanoTime();
                totalQuick += (endQuick - startQuick);

                startMerge = System.nanoTime();
                MergeSort.sort(test100);
                endMerge = System.nanoTime();
                totalMerge += (endMerge - startMerge);

                startNew = System.nanoTime();
                NewSort.sort(test100);
                endNew = System.nanoTime();
                totalNew += (endNew - startNew);
            }

            System.out.println("Average Run Time for size = " + k);
            System.out.println("Quick Sort Time: " + totalQuick / 1000 + " ns");
            System.out.println("Merge Sort Time: " + totalMerge / 1000 + " ns");
            System.out.println("New Sort Time: " + totalNew / 1000 + " ns");
            System.out.println();

        }
        /* Search Algorithms End */


    }
     /* public static void draw(Integer[] arr){
        for (int i: arr) {
            System.out.println(i + " ");
        }
    }*/
}
