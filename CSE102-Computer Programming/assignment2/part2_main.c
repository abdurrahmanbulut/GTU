#include <stdio.h>


int number_length(int number);
int find_digit(int number, int index);

//===============
// main function
//===============
int main()
{

    int number=0;

    printf("Enter a number(maximum 6 digits):\n");
    scanf("%d", &number);

    // assumed positive number
    if(number < 0) number *= -1; 

    printf("Your number has %d digits\n\n", number_length(number));

    printf("When your number is written 100 times next to each other,");
    printf(" which digit of this number would you like to see? :\n");
    
    int digit_selected=0;
    scanf("%d", &digit_selected);

    printf("%d.th digit of the big number sequence: %d", digit_selected, find_digit(number, digit_selected));

    return 0;
}
// number_length function
// it takes number which is given from command prompt
// returns length of input number
int number_length(int number)
{
    int count=1;

    while(number/10 != 0)
    {
        number /= 10;
        count++;
    }

    return count;
}
// find_digit function
// it takes a number which is given from command prompt and an index to  find number at that index.
int find_digit(int number, int index)
{
    int length_of_number = number_length(number);
    int repeated = (index % length_of_number); //to find which index of number

    int i=0;
    for(i = 0; i < length_of_number - repeated ; i++)
    {
        number /= 10;
    }
    return i;
}