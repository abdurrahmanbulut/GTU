#ifndef __SECURITY_H__
#define __SECURITY_H__


#include <string>

using namespace std;

class Security
{
public:
    static int validate(string username, string password);
};

#endif