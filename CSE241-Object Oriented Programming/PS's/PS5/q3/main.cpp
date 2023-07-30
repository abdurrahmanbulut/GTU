// main.cpp file

#include <iostream>
#include "password.h"
#include "username.h"

using namespace std;
using namespace Authenticate;

int main()
{
    inputUserName();
    inputPassword();

    cout << "Your username is " << getUserName() << " and your password is: "
    << getPassword() << endl;

    cout <<"Enter a character to exit." <<endl;
    char wait;
    cin >>wait;

    return 0;
}