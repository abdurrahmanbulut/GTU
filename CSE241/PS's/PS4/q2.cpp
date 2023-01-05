#include <iostream>
#include <vector>
#include <cstdlib>


using namespace std;

//======================
//main function
//======================
int main()
{
    //variable decleration
    int i, current, numSuitors;

    cout << "Enter the number of suitors" << endl;
    cin >> numSuitors;

    vector<int> suitors(numSuitors);

    for (int i = 0; i < numSuitors; i++)
    {
        suitors[i] = i+1; //number each suitor's position
    }


    if(numSuitors <= 0)
        cout << "Not enough suitors." << endl;
    else if(numSuitors == 1)
        cout << "You would stand first in line." << endl;
    else
    {
        current = 0; //current suitor the princes will examine
        //eliminate a suitor as long as there is at least one
        while(suitors.size() > 1)
        {   //count three people ahead, or go two people down
            for (i = 0; i < 2; i++)
            {
                current++;
                //if we reached go back to the front 
                if(current == suitors.size())
                    current = 0;
            }
            //eliminate contestant current
            suitors.erase(suitors.begin() + current);
            //if we were at the last suitor , go to the first one
            if(current == suitors.size())
                current = 0;
        }
        cout << "To win the princes, you should stand in position " << suitors[0] << endl;
    }

    return 0;
}