// administrator.h
// this is the header file for the Administrator class


#ifndef __ADMIN_H__
#define __ADMIN_H__

#include <string>
using namespace std;

class Administrator
{
    public:
        Administrator();
        Administrator(string name, string password);
        bool Login();
    private:
        string name;
        string password;
};


#endif
