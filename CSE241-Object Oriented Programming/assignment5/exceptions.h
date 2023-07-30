// Abdurrahman BULUT
//========================================
// exceptions classes that inherited from exception library
// exceptions throws from this file.

#ifndef EXCEPTIONS_H_BULUT
#define EXCEPTIONS_H_BULUT

#include <exception>

using namespace std;

// derived exception class
class DuplicateException : public exception{  

public:
    // return type of exception
    virtual const char* what() const throw()
    {  
        return "Exception : duplicate entry";
    }
};

class MissingException : public exception{

public:
    virtual const char* what() const throw()
    {
        return "Exception : missing field";
    }
};

class CommandException : public exception{

public:
    virtual const char* what() const throw()
    {
        return "Exception: command is wrong";
    }
};

#endif