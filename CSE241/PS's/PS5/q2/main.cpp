// main.cpp
// Test file to try out the user and administrator classes


#include <iostream> 
#include <string>
#include "user.h"
#include "administrator.h"

using namespace std;

int main()
{
    // Hard-coded tests. Abbott has user access, costello has user and admin access.
    User user1("abbott", "monday"), user2("lynn", "guini"), user3("costello", "tuesday");
    Administrator admin1("abbott", "monday"), admin2("kerry", "oki"), admin3("costello", "tuesday");

    cout << "Results of login:" << endl;
    cout << "User login for abbott: " << user1.Login() <<endl;
    cout << "User login for lynn: " << user2.Login() <<endl;
    cout << "User login for costello: " << user3.Login() <<endl;
    cout << "Admin login for abbott: " << admin1.Login() <<endl;
    cout << "Admin login for lynn: " << admin2.Login() <<endl;
    cout << "Admin login for costello: " << admin3.Login() <<endl;
    return 0;
}

