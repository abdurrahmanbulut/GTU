// password.cpp file

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

namespace
{
    string password;
    bool isValid()
    {
        if(password.length() >= 8)
        {
            for (int i = 0; i < password.length(); i++)
                if(!isalpha(password[i]))
                    return true;
        }
        return false;
    }
}

namespace Authenticate
{
    void inputPassword()
    {
        do
        {
            cout << "Enter your password (at least 8 charactes " <<
                    "and at least one non-letter)" << endl;
            cin >> password;
        } while (!isValid());
    }
    string getPassword()
    {
        return password;
    }

}