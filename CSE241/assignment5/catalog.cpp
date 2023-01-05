// Abdurrahman BULUT
//========================================
// catalog.cpp implementation file
// This file contains Catalog class implementations

#include <fstream>
#include <iostream>
#include "catalog_movie_music_book.h"

// default construcor. initialize vector size to zero
template<class T>
Catalog<T>::Catalog() 
{
    vectorSize = 0;
}
// it pushes item to data vector
template<class T>
void Catalog<T>::push_back(T item) 
{
    data.push_back(item);
    vectorSize++;
}
// it returns vector length
template<class T>
int Catalog<T>::size() 
{
    return vectorSize;
}
// index operator overloading.It returns the value at that index.
template<class T>
T &Catalog<T>::operator[](int index) 
{
    return data[index];
}
//  Constructor that sets catalog name
template<class T>
Catalog<T>::Catalog(const string& catalogName)
{
    this->catalogName = catalogName;
    vectorSize = 0;
}
// it reads file and gets all informations from file and store object.
template<class T>
void Catalog<T>::getDataFromFile() 
{
    ifstream dataFile;
    dataFile.open("data.txt");   // opening data.txt

    ofstream outputFile;
    outputFile.open("output.txt"); // opening output.txt

    outputFile << "Catalog Read : " << catalogName << endl; // printing catalog type to output file

    string line; // keeps data line

     // number of double quotes in the data line
    int numberOfQuotes = 0, maxNumberOfQuotes = 8;  // keeps number of max quotes in the data line

    if (catalogName == "movie")// if catalog is movie catalog
        maxNumberOfQuotes = 10;

    getline(dataFile, line);     // seek catalog type line

     // getting data line by line
    while (getline(dataFile, line)) 
    {
        try {

            for (int i = 0; i < line.length(); ++i) 
            { // counting quotes
                if (line[i] == '"') 
                    numberOfQuotes++;
            }

            if (numberOfQuotes != maxNumberOfQuotes)  // if there is missing quotes
                throw (MissingException());

            numberOfQuotes = 0; // resetting number of quotes for next data


            T temp(line); // create T type object with passing line constructor

            for (int i = 0; i < vectorSize; ++i) 
            {  // checking duplicate entry
                if (temp.getTitle() == data[i].getTitle()) // if titles are same
                    throw (DuplicateException());
                
            }

            push_back(temp); // add temp to data vector

        } catch (DuplicateException& e) 
        { // catching duplicate entry exception
            outputFile << e.what() << endl;
            outputFile << line << endl;
        }

        catch (MissingException& e) 
        {  // catching missing field exception
            outputFile << e.what() << endl;
            outputFile << line << endl;
            numberOfQuotes = 0;
        }
    }
    outputFile.close(); // closing file
    dataFile.close();
}
// It prints infos to output file using iterator.
template<class T>
void Catalog<T>::printData() 
{
    ofstream outputFile;

    outputFile.open("output.txt", ios::app);

    // print data output with using iterator
    for (auto it = data.begin(); it != data.end(); it++) 
        outputFile << *it;
    
    outputFile.close();
}
// returns data
template<class T>
vector<T>& Catalog<T>::getData() 
{
    return data;
}
//  It prints unique entry to output file.
template<class T>
void Catalog<T>::printUniqueEntry() 
{
    ofstream outputFile;
    outputFile.open("output.txt", ios::app);

    outputFile << vectorSize << " unique entries" << endl; // printing unique entry
    outputFile.close();
}


