//prime numbers between 3 to 100

#include <iostream>
#include <cstdlib>

using namespace std;
//=====================
//main function
//=====================
int main()
{
    //varialbe decleration
    bool isprime;

    //loop tests to see if i is prime for i = 3 to 100
    for (int i = 3; i <= 100; i++)
    {
        //assume i is prime
        isprime = true; 
        //inner loop tests the number is prime or not.
        //it is not necessary to get j from 2 to i-1. i/2 makes it efficient .
        for (int j = 2; j <= i/2 ; j++) 
        {
            //i is prime if i%j has no remainder
            if(i % j == 0){
                isprime = false;
                j = i;      //exit the for loop without using break keyword;
            }
        }
        //check if flag is still true
        if(isprime)
            cout << i << " is a prime number"<< endl; //output that i is prime
    }

    cout << endl;

    return 0;
}
