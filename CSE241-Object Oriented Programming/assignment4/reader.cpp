
// Abdurrahman BULUT
// Contact : abdurrahmanbulut@yahoo.com
//========================================
// reader.cpp
// This file contains readcircuit function implementation.

#include <iostream>
#include "reader.h"
#include <sstream>
#include <fstream>

#include <string>
using namespace std;

void readCircuit(string filename, int *gateCount, int *outputCount, int *inputCount)
{
    (*gateCount) = 0;
    (*inputCount) = 0;
    (*outputCount) = 0;

    stringstream ss;
    string tempStr;

    ifstream cirFile; // circuit file
    cirFile.open("circuit.txt");

    if (!(cirFile.is_open()))
        exit(1);

    while (getline(cirFile, tempStr))
    {
        ss.str(tempStr);
        ss >> tempStr;
        if (tempStr == "INPUT")
        {
            while (ss >> tempStr)
            {
                (*inputCount)++;
                (*gateCount)++;
            }
        }
        else if (tempStr == "OUTPUT")
        {
            while (ss >> tempStr)
            {
                (*outputCount)++;
                (*gateCount)++;
            }
        }
        else 
            (*gateCount)++;
        ss.clear();
    }
    cirFile.close();
}
