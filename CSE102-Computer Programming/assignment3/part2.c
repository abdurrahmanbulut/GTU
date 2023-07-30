#include <stdio.h>
#include <stdlib.h>

int isprime(int a);

int main()
{
    int number = 0;

    printf("Please enter an integer: ");
    scanf("%d", &number);

    if(number < 2)
    {
        printf("it is not in the range");
        exit(1);
    }

    int i = 0;

    for (i = 2; i < number; i++)
    {
        if (isprime(i) == -1)
            printf("%d is a prime number\n", i);
        else
            printf("%d is not a prime number, it is divisible by %d\n", i, isprime(i));
    }

    return 0;
}

int isprime(int a)
{
    int i = 0;

    if (a == 2)
        return -1; //success

    for (i = 2; i <= a / 2; i++)
    {
        if (a % i == 0)
            return i;
    }

    return -1; //success
}
