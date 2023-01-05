// user.cpp
// this is the implementation file for the user class

#include <iostream>
#include <string>
#include "user.h"
#include "security.h"

using namespace std;

//*********************
// User::User
// Constructor that initializes strings to empty
//*********************
User::User()
{
    name = "";
    password = "";
}
//*********************
// User::User
// Constructor that sets values of strings
//*********************
User::User(string username, string pass)
{
    name = username;
    password = pass;
}
//*********************
// User::Login
// This method invokes the security validate method to determine
// if the username and password should have user privileges.
// True is returned if so, False otherwise.
//*********************
bool User::Login()
{
    int result;

    result = Security::validate(name, password);
    if((result == 2) || (result == 1))
        return true;
    return false;
}