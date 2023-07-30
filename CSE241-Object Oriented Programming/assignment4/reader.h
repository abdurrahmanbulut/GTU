// Abdurrahman BULUT
// Contact : abdurrahmanbulut@yahoo.com
//========================================
// reader.h
// This file contains readcircuit function prototype.


#ifndef READER_H_BULUT
#define READER_H_BULUT

#include <string>
using std::string;


// readCirtuit function
// It opens circuit.txt file and counts gates count, outputs count and inputs count
// returns these counts via pointer usage
// Gate count includes both input and output counts. Basically all gates number.
void readCircuit(string filename, int* gateCount, int* outputCount, int* inputCount);


#endif 