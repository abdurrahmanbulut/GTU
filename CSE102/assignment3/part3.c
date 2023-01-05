#include <stdio.h>


int len(int num);
int checkBinary(int num);
int andop (int a, int b);
int myPow(int a, int b);
//******************
// main function
//****************** 
int main()
{
    int firstInt=0, secondInt=0;
    int lenFirst=0, lenSecond=0;
    do
    {
        printf("First Integer:  ");
        scanf("%d", &firstInt);
        lenFirst = len(firstInt);

        printf("Second Integer:  ");
        scanf("%d", &secondInt);
        lenSecond = len(secondInt);

        if(lenFirst != lenSecond) 
            printf("Integers should have the same length, please enter 2 new integers.\n");
        else if(checkBinary(firstInt) == 0 || checkBinary(secondInt) == 0)
            printf("Integers should be binary, please enter 2 new integers.\n");

    } while (lenFirst != lenSecond || checkBinary(firstInt) == 0 || checkBinary(secondInt) == 0);

    printf("%d AND %d = %d", firstInt, secondInt, andop(firstInt, secondInt));


    return 0;
}
//******************
// len function
// it returns the length of given number.
//****************** 
int len(int num)
{
    int len=0;

    if(num == 0) return 0;

    while(num != 0)
    {
        num /= 10;
        len++;
    }
    return len;
}
//******************
// checkBinary function
// it returns 1(true) if num is a binary number, othwerwise it returns 0 (false)
//****************** 
int checkBinary(int num)
{
    int flag = 1;

    if(num == 0) return 1;

    while(num != 0)
    {
        if(num % 10 != 0 && num % 10 != 1) 
        { 
            flag = 0;
        }
        num /= 10;
    }
    return flag;
}
//******************
// andop function
// It  calculates the result of the AND operation without using & operation and returns result.
//****************** 
int andop (int a, int b)
{
    int result=0;
    int i=0;

    while(a != 0)
    {
        if(a % 10 == b % 10 && a % 10 == 1)
            result += myPow(10, i);

        a /= 10;
        b /= 10;
        i++;
    }
    return result;
}
//******************
// myPow func
// It takes  a for base and b to calculate a^b and returns it.
//****************** 
int myPow(int a, int b)
{
    int i;

    if(b == 0)
        return 1;

    for (i = 1; i < b; i++)
    {
        a *= 10;
    }
    return a;
}
