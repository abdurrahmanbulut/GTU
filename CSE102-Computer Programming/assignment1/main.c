/*
    Abdurrahman BULUT
    Gebze Technical University
    Computer Science Engineering
    Contact:
    abdurrahmanbulut08@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

//question 1 funcs
void find_root();
int check_roots(int a, int b, int c);
//================================
//question 2 funcs
float foo(int a, int b, int c, float x);
float findDerivative(int a, int b, int c, float x);
void find_newtonian_root();
//================================
//question3 funcs
void print_roots(int a, int b, int c);
void find_multiple_closest();
int check_closest_number(int num1, int num2);


//=====================
// main function
//=====================
int main()
{
    find_root();
    find_newtonian_root();
    find_multiple_closest();

    return 0;
}
//======================================
//question 1
//find_root function
//It takes 3 integer input from user and finds the roots or informs status of root existincy
void find_root()
{
    int firstCoefficient=0, secondCoefficient=0, thirdCoefficient=0, i=0;

    printf("Please Enter the first coefficient:");
    scanf("%d", &firstCoefficient);
    printf("Please Enter the second coefficient:");
    scanf("%d", &secondCoefficient);
    printf("Please Enter the third coefficient:");
    scanf("%d", &thirdCoefficient);

    (firstCoefficient != 0) ? printf("Your equation %dx^2", firstCoefficient) : printf("Your equation ");
    
    if(secondCoefficient != 0)
        (secondCoefficient < 0 ) ? printf("%dx", secondCoefficient) : printf("+%dx", secondCoefficient);
    
    else
        printf("+%dx", secondCoefficient);


    if(thirdCoefficient != 0)
        (thirdCoefficient < 0 ) ? printf("%d", thirdCoefficient) : printf("+%d", thirdCoefficient);

    if(check_roots(firstCoefficient, secondCoefficient, thirdCoefficient) != 0)
        (check_roots(firstCoefficient, secondCoefficient, thirdCoefficient) < 0 ) ? printf(" have complex roots") : print_roots(firstCoefficient, secondCoefficient, thirdCoefficient);
    
    else
        printf(" does not have any real roots");

    printf("\n");
}   
//check_roots function
//inputs : ax^2 + bx + c, it takes a, b and c from equation
//it returns 1 if there are real root(s), otherwise it returns 0
int check_roots(int a, int b, int c)
{
    return (b*b - 4*a*c >= 0);
}
//print_roots function
//inputs:  ax^2 + bx + c, it takes a, b and c from equation
//It prints roots
void print_roots(int a, int b, int c)
{
    int delta = b*b - 4*a*c;

    int root1 = (-1.0*b + sqrt(delta))/(2*a);
    int root2 = (-1.0*b - sqrt(delta))/(2*a);

    printf(" have real roots {%d, %d}.", root1, root2);
}
//=========================================
//question 2
// func: find_newtonian_root
// it asks 4 input from user and apply newtonian algorithm for 5 times with initial number.
/*
Step	x	F(x)	Difference
x1	6.1176	15.0727	3.8823
x2	4.4856	2.6637	1.6321
x3	4.0395	0.1990	0.4461
x4	4.0003	0.0015	0.0392
x5	4.0000	0.0000	0.0003
x6	4	0	0.0000  0.0000
*/
void find_newtonian_root()
{
    int firstCoefficient=0, secondCoefficient=0, thirdCoefficient=0, i=0;
    float x1=0.0, x0=0.0;

    printf("Please Enter the first coefficient:");
    scanf("%d", &firstCoefficient);
    printf("Please Enter the second coefficient:");
    scanf("%d", &secondCoefficient);
    printf("Please Enter the third coefficient:");
    scanf("%d", &thirdCoefficient);
    printf("Please Enter the initial:");
    scanf("%f", &x0);
    
    printf("\n%s\t %s\t %s\t %s\n\n", "Step", "x", "f(x)", "Diference");

    for (i = 1; i <= 5; i++)
    {   
        printf("x%d\t", i);
        //x1 = x0 - f(x0)/f'(x0) --> newtonian eq.
        x1 = x0 - (foo(firstCoefficient, secondCoefficient,thirdCoefficient, x0) / findDerivative(firstCoefficient, secondCoefficient, thirdCoefficient, x0));
        
        printf("%.4f\t", x1);
        printf("%.4f\t", foo(firstCoefficient, secondCoefficient, thirdCoefficient, x1));
        printf("%.4f", x0-x1);
        x0 = x1;
        printf("\n");
    }
}
//func: foo
//inputs: a, b, c  ax^2 + bx + c equation. x is initial number
//return: float result of equation
float foo(int a, int b, int c, float x)
{
    return (a*x*x + b*x + c);
}
//func: fooDerivative
//inputs: a, b, c  ax^2 + bx + c equation. x is initial number
//return: float result of derivation of equation
float findDerivative(int a, int b, int c, float x)
{
    return (2*a*x + b);
}
//===============================================
// question 3
// divisibility of the first integer by the second one. If
// it is not, it finds the closest divisible integer
void find_multiple_closest()
{
    int firstInt=0, secondInt=0;

    printf("Enter the first integer:");
    scanf("%d", &firstInt);
    printf("Enter the second integer:");
    scanf("%d", &secondInt);

    int closestNum = check_closest_number(firstInt, secondInt);
    (closestNum != 0) ? printf("Closest number to %d that is multiple of %d is %d", firstInt, secondInt, closestNum) 
                      : printf("Closest number to %d that is multiple of %d is itself", firstInt, secondInt);
    
    printf("\n");
}
// if num1 is divisible by num2, returns 0
// otherwise it returns closest number which is divisible by num2.
int check_closest_number(int num1, int num2)
{   
    if(num1 % num2 == 0)
        return 0;
    
    else if(num1 % num2 < (num2 / 2))
        return (num1 - num1 % num2);

    else
        return (num1 + num2 - num1 % num2);
}
