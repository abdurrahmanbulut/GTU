#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

const int NUM_DUELS = 10000;

//Function prototypes

void Aaron_shoots(bool aaron, bool& bob, bool& charlie);
void Bob_shoots(bool& aaron, bool bob, bool& charlie);
void Charlie_shoots(bool& aaron, bool& bob, bool charlie);

//aaron shoots
void Aaron_shoots(bool aaron, bool& bob, bool& charlie)
{
    int shot;
    //If aaron is dead, do nothig
    if(aaron == false) 
        return;
    
    //aaron's shot
    shot = rand() % 3; //1/3 chance
    
    if(shot==0)
    {   //would shoot at charlie if still alive
        if(charlie)  charlie = false; //set to dead
        //would shoot at bob second
        else if(bob) bob = false;//set to dead
    }
    return;
}
//Bob shoots, Bob's turn.
void Bob_shoots(bool& aaron, bool bob, bool& charlie)
{
    int shot;
    //If bob is dead, do nothig
    if(bob == false) 
        return;
    
    //bob's shot
    shot = rand() % 2; //1/2 chance
    
    if(shot==0)
    {   //would shoot at charlie if still alive
        if(charlie) charlie = false; //set to dead
        //would shoot at aaron second
        else if(aaron) aaron = false;//set to dead
    }
    return;
}
//Charlie shoots, charli's turn
void Charlie_shoots(bool& aaron, bool& bob, bool charlie)
{
    int shot;
    //If Charlie is dead, do nothig
    if(charlie == false) 
        return;

    //charlie never misses
    if(bob)  bob = false; //set to dead
    else if(aaron) aaron = false;
    
    return;
}
//=======================
//Main function
//=======================
int main()
{
    //variable declerations
    bool aaronAlive, bobAlive, charlieAlive;
    int aaronWins = 0;
    int bobWins = 0;
    int charlieWins = 0;
    int i;
    //Initialize random number generator using current clock
    srand(time(NULL));
    //play 10000 duels
    for(i=0; i<NUM_DUELS; i++)
    {
        aaronAlive = true;
        bobAlive = true;
        charlieAlive = true;
        //continue as long as two people are alive
        do
        {
            Aaron_shoots(aaronAlive, bobAlive, charlieAlive);
            Bob_shoots(aaronAlive, bobAlive, charlieAlive);
            Charlie_shoots(aaronAlive, bobAlive, charlieAlive);

        } while (((aaronAlive==true) && (bobAlive==true)) ||
                  ((aaronAlive==true) && (charlieAlive==true)) ||
                  ((bobAlive==true) && (charlieAlive==true)));
        
        if(aaronAlive) aaronWins++;
        if(bobAlive) bobWins++;
        if(charlieAlive) charlieWins++;
    }
    cout.setf(ios::fixed);          
    cout.setf(ios::showpoint);
    cout.precision(2);

    //output probability of winning
    cout << "Aaron won " << aaronWins << "/10000 duels or "<<
        (static_cast<double>(aaronWins) / NUM_DUELS)*100 << "%" <<endl;
    cout << "Bob won " <<bobWins << "/10000 duels or "<<
        (static_cast<double>(bobWins) / NUM_DUELS)*100 << "%" <<endl;
    cout << "Charlie won " << charlieWins << "/10000 duels or "<<
        (static_cast<double>(charlieWins) / NUM_DUELS)*100 << "%" <<endl;
}

