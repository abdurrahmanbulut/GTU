#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>

using namespace std;
//function prototypes
bool checkUnique(string secretNumber);
int findExact(string guess, string secretNumber);
int findMisplaced(string guess, string secretNumber);
bool isInteger(string number);
int myPow(int bottom, int top);
//=================
//Main function
//=================
int main(int argc, char **argv)
{
    // to avoid logic_error of construct null. checkRandU = argv[1].
    if(argc != 3) 
    {
        cerr << "E0" << endl;
        exit(1);
    }
    //variable decleration
    string checkRandU = argv[1], guess;
    string secretNumber = argv[2];
    int IterationCounter = 1, countExact = 0;

    //check error like wrong parameters ,mising parameters , uniquness 
    if (  argv[2][0] == '0' || argv[2][0] == '-' ||  !(checkRandU == "-r" || checkRandU == "-u") || checkUnique(secretNumber))
    {
        cerr << "E0" << endl;
        exit(1);
    }
    //check value after -u for integer
    if (checkRandU == "-u")
    {
        if (!isInteger(secretNumber) )
        {
            cerr << "E0" << endl;
            exit(1);
        }

        do
        {
            //input from user to guess
            cin >> guess;
            //checks guess errors if they are.
            if (!(isInteger(guess)))
            {
                cerr << "E2" << endl;
                exit(1);
            }
            else if (secretNumber.length() != guess.length() || checkUnique(guess))
            {
                cerr << "E1" << endl;
                exit(1);
            }
            else if( guess[0] == '0')
            {
                cerr << "E0" << endl;
                exit(1);
            }
            //The count of digits of the proposed number that match in place of the secret number.
            countExact = findExact(guess, secretNumber); 

            if (countExact == secretNumber.length())
            {
                cout << "FOUND " << IterationCounter << endl;
                exit(1);
            }
            else
            {
                cout << countExact << ' ' << findMisplaced(guess, secretNumber) << endl;
            }
            IterationCounter++;
            //if user cannot solve in 100 iteration, the game ends.
        } while (IterationCounter <= 100);

        cout << "FAILED" << endl;
        exit(1);
    }
    else if (checkRandU == "-r")
    {
        //variable decleration
        int secretNum = 0;
        //initialize random int number generator using the current value of the clock 
        srand(static_cast<int>(time(NULL)));
        //string to int
        int digitOfNum = secretNumber[0] - '0';
        
        if (secretNumber[1] != '\0') //N <= 9
        {
            cerr << "E0" << endl;
            exit(1);
        }

        do
        {//this eq. generates number which has digitOfNum digits.
            secretNum = rand() % (myPow(10, digitOfNum) - digitOfNum * myPow(10, digitOfNum - 1)) + myPow(10, digitOfNum - 1);
            secretNumber = to_string(secretNum);

        } while (checkUnique(secretNumber));

        do
        {   //input from user to guess
            cin >> guess;
            //checks guess errors if they are.
            if (!(isInteger(guess)))
            {
                cerr << "E2" << endl;
                exit(1);
            }
            else if (secretNumber.length() != guess.length() || checkUnique(guess))
            {
                cerr << "E1" << endl;
                exit(1);
            }
            else if(guess[0] == '0')
            {
                cerr << "E0" << endl;
                exit(1);
            }
            //The count of digits of the proposed number that match in place of the secret number.
            countExact = findExact(guess, secretNumber); 

            if (countExact == secretNumber.length())
            {
                cout << "FOUND " << IterationCounter << endl;
                exit(1);
            }
            else
            {
                cout << countExact << ' ' << findMisplaced(guess, secretNumber) << endl;
            }
            IterationCounter++;
        //if user cannot solve in 100 iteration, the game ends.
        } while (IterationCounter <= 100);

        cout << "FAILED" << endl;
        exit(1);
    }

    return 0;
}
/*
checkUnique(string secretNumber)
it checks string for uniqueness
input: 
secretNumber: Number of giving by user or automatic produced . It may be other string or numbers for uniqueness checking 
returns: if string is unique, then it returns false
*/
bool checkUnique(string secretNumber)
{

    for (int i = 0; secretNumber[i] != '\0'; i++)
    {
        for (int j = i + 1; secretNumber[j] != '\0'; j++)
        {
            if (secretNumber[i] == secretNumber[j])
                return true;
        }
    }
    return false;
}
/*
findExact(string guess, string secretNumber)
it finds exact matches count among guess number and secretNumber
inputs:
guess: user prediction
secretNumber: Number of giving by user or automatic produced
returns: exacth matched numbers
*/
int findExact(string guess, string secretNumber)
{
    int counter = 0;

    for (int i = 0; secretNumber[i] != '\0'; i++)
    {
        if (guess[i] == secretNumber[i])
        {
            counter++;
        }
    }
    return counter;
}
/*
it finds misplaced matches count among guess number and secretNumber
inputs:
guess: user prediction
secretNumber: Number of giving by user or automatic produced
returns: missplaced matched numbers
*/
int findMisplaced(string guess, string secretNumber)
{
    int counter = 0;

    for (int i = 0; secretNumber[i] != '\0'; i++)
    {
        for (int j = 0; secretNumber[j] != '\0'; j++)
        {
            if (i == j)
                continue;

            if (secretNumber[i] == guess[j])
                counter++;
        }
    }
    return counter;
}
/*
isInteger(string number)
it checks for integer
if number is integer, it will return true.
input:
number: number to check
returns: true if it is integer, otherwise false
*/
bool isInteger(string number)
{
    for (int i = 0; i < number.length(); i++)
    {
        if (number[i] < '0' || number[i] > '9')
        {
            return false;
        }
    }
    return true;
}
/*
myPow(int bottom, int top)
it calculates bottom over top.
input:
bottom: x in x^y
top:    y in x^y
returns: result of calculation 
*/
int myPow(int bottom, int top)
{
    int result = 1;

    for (int i = 0; i < top; i++)
    {
        result *= bottom;
    }
    return result;
}