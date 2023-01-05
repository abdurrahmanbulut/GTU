#include <stdio.h>


int sum (int n, int flag);
int mult (int n, int flag);
//*************************
// main funciton
//*************************

int main()
{
    // range -> [1,N]
    int N=0;

    printf("Enter an integer: ");
    scanf("%d", &N);

    // decide if it is going to work on sum / multiplication..
    int operationFlag=-1;
    printf("Please enter '0' for sum, '1' for multiplication\n");
    scanf("%d", &operationFlag);
    
    // decide if it is going to work on odd / even numbers.
    int addMultFlag = -1;
    printf("Please enter '0' to work even numbers, '1' to work on odd numbers\n");
    scanf("%d", &addMultFlag);

    switch (operationFlag)
    {
    case 0:
        sum(N, addMultFlag);
        break;
    case 1:
        mult(N, addMultFlag);
        break;
    default:
        printf("Unsupported operation.\n");
        break;
    }

    return 0;
}
//****************
// sum dunction
// it prints numbers and find sum of them by looking flag number to decide even or odd operations.
//****************
int sum (int n, int flag)
{   
    int sum=0;
    int i=0;

    switch (flag)
    {
    case 0: // even

        for (int i = 2; i <= n; i+=2)
        {
            sum += i;
            (i == n || i == n-1) ? printf("%d", i)  : printf("%d+", i);
        }
        printf("=%d", sum); 
        break;

    case 1: //odd

        for (int i = 1; i <= n; i+=2)
        {
            sum += i;
            (i == n || i == n-1) ? printf("%d", i) : printf("%d+", i);
        }
        printf("=%d", sum);
        break;

    default:
        printf("Invalid value for odd/even selection.\n");
        break;
    }
}
//****************
// mult function
// it prints numbers and find multiplication of them by looking flag number to decide even or odd operations.
//****************
int mult (int n, int flag)
{   
    int mult=1;
    int i=0;

    switch (flag)
    {
    case 0: // even
        for (int i = 2; i <= n; i+=2)
        {
            mult *= i;
            (i == n || i == n-1) ? printf("%d", i) : printf("%d+", i);
        }
        printf("=%d", mult);
        break;
    case 1://odd

        for (int i = 1; i <= n; i+=2)
        {
            mult *= i;
            (i == n || i == n-1) ? printf("%d", i) : printf("%d+", i);
        }
        printf("=%d", mult);
        break;

    default:
        printf("Invalid value for odd/even selection.\n");
        break;
    }
}
