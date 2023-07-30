// user.h
// this is the header file for the user class.

#ifndef __USER_H__
#define __USER_H__

#include <string>
using namespace std;

class User
{
    public:
        User();
        User(string name, string password);
        bool Login();
    private:
        string name;
        string password;
};

#endif // __USER_H__
