#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


void decrypt_and_print (char* file_path);
char decrypt_numbers (int number);
void deep_decrypt_and_print (char* file_path);
void refresh_position (int *X, int *Y, double *D, double *R);
void track_machine ();
void encrypt_messages (char* file_path);
void menu();

//****************
// main function
//****************
int main()
{  
    menu();

    return 0;
}
//***********************************************************************
// decrypt_and_print function
// it takes filename path to open. filename -> encrypted_p1.img 
// it finds the secret message by looking at numbers which is in file.
// 0, 1, 2, 3, 4, 5, 6 are ‘ ’, ‘-’, ‘_’, ‘|’, ‘/’, ‘\’, ‘O‘ respectively
// prints result to the console
//************************************************************************
void decrypt_and_print (char* file_path)
{
    FILE* p;
    p = fopen(file_path, "r");
    
    if(p == NULL)
        exit(1);

    char temp;
    int temp2; // to send decrypt_numbers

    fscanf(p, "%c", &temp);
    

    while (temp != EOF)
    {
        if(feof(p)) break;

        temp2 = temp - 48;

        if(temp == '\n')
            printf("%c", '\n');
        else   
            printf("%c", decrypt_numbers(temp2));
        
        fscanf(p, "%c", &temp);
        temp2 = temp - 48;
    }

    fclose(p);
}
//***********************************************************************
// decrypt_numbers function
// it takes number to convert this number to the character
// 0, 1, 2, 3, 4, 5, 6 are ‘ ’, ‘-’, ‘_’, ‘|’, ‘/’, ‘\’, ‘O‘ respectively
// returns appropriate character
//************************************************************************
char decrypt_numbers (int number)
{
    if(number == 0)
        return ' ';
    else if(number == 1)
        return '-';
    else if(number == 2)
        return '_';
    else if(number == 3)
        return '|';
    else if(number == 4)
        return '/';
    else if(number == 5)
        return '\\';
    else if(number == 6)
        return 'O';
}
//***********************************************************************
// deep_decrypt_and_print function
// it takes filename path to open. filename -> encrypted_p2.img
// it finds the secret message by looking at numbers which is in file.
// 0 1 2 0 5 1 2 0 5 ...
// 0+1+2 -> 3 % 7 = 3
// 1+2+0 -> 3 % 7 = 4
// 2+0+5 -> 7 % 7 = 0
// new message -> 3 4 0 .....
// prints result to the console
//************************************************************************
void deep_decrypt_and_print (char* file_path)
{
    FILE* p;
    p = fopen(file_path, "r");

    if(p == NULL)
    {
        exit(1);
        printf("File is empty!");
    }
    
    int num1=0, num2=0, num3=0;

    num1 = getc(p) - 48;
    num2 = getc(p) - 48;
    num3 = getc(p) - 48;

    while(!feof(p)){ 
        
        if(num1 != '\n' - 48 && num2 != '\n' - 48 && num3 != '\n' - 48)
            printf("%c", decrypt_numbers((num1 + num2 + num3) % 7));

        num1 = getc(p) - 48;
        if(num1 == '\n' - 48)
        {
            printf("\n");
            continue;
        }

        num2 = getc(p) - 48; 
        if(num2 == '\n' - 48)
        {
            printf("%c", decrypt_numbers((num1) % 7));
            printf("\n");
            continue;
        }

        num3 = getc(p) - 48;
        if(num3 == '\n' - 48)
        {
            printf("%c", decrypt_numbers((num1 + num2) % 7));
            printf("%c", decrypt_numbers((num2) % 7));
            printf("\n");
        } 
    }    
    printf("%c", decrypt_numbers((num1 + num2) % 7));
    printf("%c", decrypt_numbers((num2) % 7));
    printf("\n");
    

    fclose(p);
}
//***********************************************************************
// track_machine function
// E represents the enemies’ current position. X and Y is the
// current axes of the position of the enemy camp. R shows the distance between current position and
// the last position of the enemy camp. D is the distance of enemy to our camp.
// prints map to the console
//************************************************************************
void track_machine ()
{
    int i, j;
    int enemyX=0, enemyY=0;
    double distanceToOurCamp=0.0;
    double displacement=0.0;
    int flag = 1;
    char command=' ';
    fflush(stdin);

    while(flag)
    {
        for (i = 1; i <= 11; i++)
        {
            for (j = 1; j <= 11; j++)
            {
                if (i == 6 && j == 6)
                {
                    printf("%c", 'O');
                }
                else if (j == enemyX && i == enemyY)
                {
                    printf("%c", 'E');
                }
                else
                {
                    printf("%c", '.');
                }
            }
            printf("\n");
        }
        distanceToOurCamp = sqrt(pow(enemyX-6, 2) + pow(enemyY-6, 2));
        
        printf("Enemies X position: %d, Y position: %d, Displacement: %.2lf, Distance to our camp: %.2lf\n", enemyX, enemyY, displacement, distanceToOurCamp);
        printf("Command waiting...:");
        scanf("%c", &command);

        if(command == 'E' || command == 'e')
        {
            printf("Exitted!");
            flag = 0;
        }
        else if(command == 'R' || command == 'r')
        {
            refresh_position(&enemyX, &enemyY, &displacement, &distanceToOurCamp);
        }
        else
            printf("Wrong command");
        fflush(stdin);
    }   
}
//***********************************************************************
// refresh_position function
// it reproduce enemy position randomly and finds displacement and distance to our camp
//************************************************************************
void refresh_position (int *X, int *Y, double *D, double *R)
{
    int x, y;
    srand(time(NULL));
    
    do
    {
        x = rand() % 11 + 1;
        y = rand() % 11 + 1;
    } while (x == 6 && y == 6);

    (*R) = sqrt(pow((*X)-x, 2) + pow((*Y)-y, 2));
    (*D) = sqrt(pow((*X)-6, 2) + pow((*Y)-6, 2));
    (*X) = x;
    (*Y) = y;
}
//***********************************************************************
// decrypt_chars function
// it takes character to convert this character to the number
// 0, 1, 2, 3, 4, 5, 6 are ‘ ’, ‘-’, ‘_’, ‘|’, ‘/’, ‘\’, ‘O‘ respectively
// returns appropriate number
//************************************************************************
int decrypt_chars (char character)
{
    if(character == ' ')
        return 0;
    else if(character == '-')
        return 1;
    else if(character == '_')
        return 2;
    else if(character == '|')
        return 3;
    else if(character == '/')
        return 4;
    else if(character == '\\')
        return 5;
    else if(character == 'O')
        return 6;
}
//***********************************************************************
// encrypt_messages function
// it takes filename path to open. filename -> decrypted_p4.img
// it finds the secret numbers by looking at secret message which is in file.
// 0 1 2 0 5 1 2 0 5 ...
// 0+0+0 -> 0 % 7 = 0
// 0+0+1 -> 1 % 7 = 1
// 0+1+2 -> 3 % 7 = 3
// new message -> 0 1 3 .....
// prints result to the file which name is  encrypted_p4.img
//************************************************************************
void encrypt_messages (char* file_path)
{
    FILE* pr;
    FILE* pw;

    pr = fopen(file_path, "r");
    pw = fopen("encrypted_p4.img", "w");
    
    if(pr == NULL)
        exit(1);

    char temp1, temp2, temp3;
    int flag = 1;


    while (!feof(pr))
    {
        if (flag == 1)
        {
            temp1 = getc(pr);
            if(feof(pr)) break;
            fprintf(pw, "%d", decrypt_chars(temp1) % 7);
            
            temp2 = getc(pr);
            if(feof(pr)) break;
            fprintf(pw, "%d", (decrypt_chars(temp1) + decrypt_chars(temp2)) % 7);
            
            temp3 = getc(pr);
            if(feof(pr)) break;
            fprintf(pw, "%d", (decrypt_chars(temp1) + decrypt_chars(temp2) + decrypt_chars(temp3)) % 7);
            flag = 0;
        }
        temp1 = getc(pr);
        if(temp1 == '\n') {
            fprintf(pw, "\n");
            flag = 1; continue;
        }
    
        fprintf(pw, "%d", (decrypt_chars(temp1) + decrypt_chars(temp2) + decrypt_chars(temp3)) % 7);

        temp2 = getc(pr); 
        if(temp2 == '\n') {
            fprintf(pw, "\n");
            flag = 1; continue;
        }

        fprintf(pw, "%d", (decrypt_chars(temp1) + decrypt_chars(temp2) + decrypt_chars(temp3)) % 7);

        temp3 = getc(pr);
        if(temp3 == '\n') {
            fprintf(pw, "\n");
            flag = 1; continue;
        }

        fprintf(pw, "%d", (decrypt_chars(temp1) + decrypt_chars(temp2) + decrypt_chars(temp3)) % 7);
    }
    fclose(pw);
    fclose(pr);
}
//***********************************************************************
// menu function
// It  operates all the three parts
// It works forever until the operator selects the exit option.
//************************************************************************
void menu()
{
    int flag = 1, choice=0;

    while (flag)
    {
        printf("1-) Decrypt and print encrypted_p1.img\n2-) Decrypt and print encrypted_p2.img\n3-) Switch on the tracking machine\n");
        printf("4-) Encrypt the message\n5-) Switch off\n\n");
        printf("Please make your choice:\n");

        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
             decrypt_and_print("encrypted_p1.img");
             break;
        case 2:
            deep_decrypt_and_print("encrypted_p2.img");
            break;
        case 3:
            track_machine();
            break;
        case 4:
            encrypt_messages("decrypted_p4.img");
            break;
        case 5:
            printf("Switched off!");
            flag = 0;
            break;
        default:
            printf("Wrong selection, try again!");
        }
    }
}