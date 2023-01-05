#include <stdio.h>

int main()
{
    // makes constant currencies 
    const float EURO_TL = 6.69;
    const float DOLLAR_TL = 6.14;
    const float EURO_DOLLAR = 6.69/6.14;

    printf("***** Welcome to ABC Exchange Office *****\n\n");
    printf("Enter your amount:\n");

    float customer_money_amount=0.0;
    scanf("%f", &customer_money_amount);

    printf("\nPlease select your currency\n");
    printf("1. Turkish Lira\n");
    printf("2. Euro\n");
    printf("3. Dollar\n\n");

    int customer_money_currency=0;
    scanf("%d", &customer_money_currency);

    int convertion_type=0;
    switch (customer_money_currency)
    {
    
    case 1:
        printf("You have %f Turkish Liras\n", customer_money_amount);
        printf("Choose which currency you want to convert\n");

        
        scanf("%d", &convertion_type);
        
        if(convertion_type == 1){
            printf("You have %f Turish Liras", customer_money_amount);
        }
        else if(convertion_type == 2){
            printf("You have %f Euro", customer_money_amount / EURO_TL);
        }
        else if(convertion_type == 3){
            printf("You have %f Dollar", customer_money_amount / DOLLAR_TL);
        }
        else
            printf("Your selection is invalid!");
        break;
    case 2:
        printf("You have %f Euros\n", customer_money_amount);
        printf("Choose which currency you want to convert\n");

        scanf("%d", &convertion_type);
        
        if(convertion_type == 1){
            printf("You have %f Turish Liras", customer_money_amount * EURO_TL);
        }
        else if(convertion_type == 2){
            printf("You have %f Euro", customer_money_amount);
        }
        else if(convertion_type == 3){
            printf("You have %f Dollar", customer_money_amount * EURO_DOLLAR);
        }
        else
            printf("Your selection is invalid!");
        break;
    case 3:
        printf("You have %f Dollars\n", customer_money_amount);
        printf("Choose which currency you want to convert\n");

        scanf("%d", &convertion_type);
        
        if(convertion_type == 1)
            printf("You have %f Turish Liras", customer_money_amount * DOLLAR_TL);
        else if(convertion_type == 2)
            printf("You have %f Euro", customer_money_amount / EURO_DOLLAR);
        else if(convertion_type == 3)
            printf("You have %f Dollar", customer_money_amount);
        else
            printf("Your selection is invalid!");
        break;
    default:
        printf("Your selection is invalid!");
        break;
    }
    return 0;
}