//administrator.cpp
//This is implementation file for the administrator class

#include <iostream>
#include <string>
#include "administrator.h"
#include "security.h"

using namespace std;

//*******************
// Administrator::Administrator
// Constructor that initializes strings to empty
//*******************
Administrator::Administrator()
{
    name = "";
    password = "";
}
//*******************
// Administrator::Administrator
// Constructor that sets values of strings
//*******************
Administrator::Administrator(string username, string pass)
{
    name = username;
    password = pass;
}
//*******************
// Administrator::Login
// It invokes the security validate method to determine if username
// and password should have privileges. True is returned if so, False otherwise.
//*******************
bool Administrator::Login()
{
    int result;

    result = Security::validate(name, password);
    
    if(result == 2)
        return true;
    return false;
}