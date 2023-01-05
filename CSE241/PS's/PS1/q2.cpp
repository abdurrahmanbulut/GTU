#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;
//function prototypes
int rollDice();
int humanTurn(int humanTotalScore);
int computerTurn(int computerTotalScore);

//generates random number between 1 to 6
int rollDice()
{
    return rand() % 6 + 1;
}
//humanTurn(int humanTotalScore)
//handles the logic for the human's turn at the pig game
//input : 
//humanTotalScore: human's score at the beginning of the turn
//returns: new value scored by the human on this turn
int humanTurn(int humanTotalScore)
{
    int score=0;
    char cInput='0';
    bool bTurnOver=false;
    
    cout << "It's your turn! Enter 'r' to roll." << endl;
    cin >> cInput; //it's just for waiting to see output message.

    do
    {
        int roll = rollDice();
        cout << "You rolled " << roll << endl;

        if (roll == 1)
        {
            cout << "you loss your turn. " << "Enter 'c' to continue." << endl;
            cin >> cInput;
            return 0;
        }
        else
        {
            score += roll;
            cout << "Your score this round is: " << score << endl 
                 << "If you hold, your total score would be: " << humanTotalScore + score << endl
                 << "Press 'h' to hold or 'r' to roll again." << endl;
            cin >> cInput;

            if (cInput == 'h')
                bTurnOver = true;
        }
    }while(!bTurnOver); 

    return score;   
}
/*
computerTurn(int computerTotalScore)
handles the logic for the computer's turn at the pig game

input: computerTotalScore : computer's score at the beginning of the turn

returns: new value after turn
*/
int computerTurn(int computerTotalScore)
{
    int score=0;
    bool bTurnOver=false;
    char cInput='0';

    cout << "It's the computer's turn!" << endl;

    do
    {
        int roll = rollDice();
        cout << "The computer rolled!" << roll << endl;

        if(roll == 1)
        {
            cout << "The computer loses its turn." << "Enter 'c' to continue." << endl;
            cin >> cInput;
            return 0;
        }
        else
        {
            score += roll;
            cout << "The computer's score this round is: " << score << endl
                 << "If the computer holds, its total score would be: " << computerTotalScore + score << endl;
            
            if ((score >= 20) || (score + computerTotalScore >= 100))
            {
                cout << "Computer holds. " << endl << "Enter 'c' to continue." << endl;
                cin >> cInput;
                bTurnOver = true;
            }
        }
    } while (!bTurnOver);
    
    return  score;

}
//=========================
//Main function
//=========================
int main()
{
    //variable decleration
    int humanScore=0, computerScore=0;
    bool gameOver=false;
    //initialize random number generator using the current value of the clock
    srand(static_cast<int>(time(NULL)));

    while(!gameOver)
    {   //human moves first
        humanScore += humanTurn(humanScore);
        //then human moves is game is not ended
        if(humanScore < 100)
            computerScore += computerTurn(computerScore);
        //check for game over
        if(humanScore >= 100)
        {
            cout << "Congratulations, you win!" << endl;
            gameOver = true;
        }
        else if (computerScore >= 100)
        {
            cout << "Sorry, the computer won." << endl;
            gameOver = true;
        }
        
    }
    return 0;

}