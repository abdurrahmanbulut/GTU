#include <iostream>
#include <cstdlib>

using namespace std;

class HotDogStand
{
public:
    HotDogStand();
    HotDogStand(int newID, int newNnumSold);
    int GetID();
    void SetID(int newID);
    void JustSold();
    int GetNumSold();
    static int GetTotalSold();

private:
    static int totalSold;
    int numSold;
    int ID;
};

int HotDogStand::totalSold = 0;


//============================
//HotDogStand::HotDogStand
//Default constructor that initialize numsold and ID to zero.
//============================
HotDogStand::HotDogStand()
{
    numSold = 0;
    ID = 0;
}
//============================
//HotDogStand::HotDogStand
//Constructor that initialize numsold and ID by arguments
//============================
HotDogStand::HotDogStand(int newID, int newNnumSold)
{
    numSold = newNnumSold;
    ID = newID;
}
//============================
//int HotDogStand::GetID
//Return ID number
//============================
int HotDogStand::GetID()
{
    return ID;
}
//============================
//void HotDogStand::SetID
//set ID number to newID
//============================
void HotDogStand::SetID(int newID)
{
    ID = newID;
}
//============================
//void HotDogStand::JustSold
//Increment the number of hotdogs in stand
//============================
void HotDogStand::JustSold()
{
    numSold++;
    totalSold++;
}
//============================
//int HotDogStand::GetNumSold
//returns the number of hotdogs which is sold in this stand
//============================
int HotDogStand::GetNumSold()
{
    return numSold;
}
//============================
//int HotDogStand::GetTotalSold
//returns the number of hotdogs sold by all stands
//============================
int HotDogStand::GetTotalSold()
{
    return totalSold;
}
//============================
//main function
//============================
int main()
{
    //test with 3 hot dogs stands
    HotDogStand s1(1, 0), s2(2, 0), s3(3, 0);

    //sold at stand 1 and 2
    s1.JustSold();
    s2.JustSold();
    s1.JustSold();

    cout << "Stand " << s1.GetID() << " sold " << s1.GetNumSold() << endl;
    cout << "Stand " << s2.GetID() << " sold " << s2.GetNumSold() << endl;
    cout << "Stand " << s3.GetID() << " sold " << s3.GetNumSold() << endl;
    cout << "Total sold = " << s1.GetTotalSold() << endl;
    cout <<endl;

    //sold some more
    s3.JustSold();
    s1.JustSold();

    cout << "Stand " << s1.GetID() << " sold " << s1.GetNumSold() << endl;
    cout << "Stand " << s2.GetID() << " sold " << s2.GetNumSold() << endl;
    cout << "Stand " << s3.GetID() << " sold " << s3.GetNumSold() << endl;
    cout << "Total sold = " << s1.GetTotalSold() << endl;
    cout <<endl;

    return 0;
}