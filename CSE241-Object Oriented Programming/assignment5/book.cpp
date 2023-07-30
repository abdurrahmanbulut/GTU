// Abdurrahman BULUT
//========================================
// book.cpp implementation file
// This file contains Book class implementations

#include <iostream>
#include "catalog_movie_music_book.h"

// Default constructor
Book::Book()
{
    title = "";
    authors = "";
    year = "";
    tags = "";
}

// Other Constructor. It store quotes indexes
Book::Book(const string& line)
{
    int quetosIndexes[8]; // keeps double quetos index in the data line
    int indexCounter = 0;

    for (int j = 0; j < line.length(); ++j)
    { // finding indexes from data line

        if (line[j] == '"')
            quetosIndexes[indexCounter++] = static_cast<int>(j);
        
    }
    // getting pieces from data line
    title = line.substr(quetosIndexes[0] + 1, quetosIndexes[1] - quetosIndexes[0] - 1);
    authors = line.substr(quetosIndexes[2] + 1, quetosIndexes[3] - quetosIndexes[2] - 1);
    year = line.substr(quetosIndexes[4] + 1, quetosIndexes[5] - quetosIndexes[4] - 1);
    tags = line.substr(quetosIndexes[6] + 1, quetosIndexes[7] - quetosIndexes[6] - 1);
}
// getter for title name.
const string& Book::getTitle() const
{
    return title;
}
// getter for author name.
const string& Book::getAuthors() const
{
    return authors;
}
// getter for year value.
const string& Book::getYear() const
{
    return year;
}
// getter for tag names.
const string& Book::getTags() const
{
    return tags;
}
// Stream insertion operator overloading
ostream& operator<<(ostream& outputStream, const Book& book)
{

    outputStream << '"' << book.title << "\" \"" << book.getAuthors() << "\" \"" << book.getYear() << "\" \"" 
                        << book.getTags() << '"' << endl;

    return outputStream;
}
// it searches str in field. If it founds, returns true.
bool Book::search(const string& str, const string& field)
{

    int found;

    if (field == "title")
    { // finding str in the field
        found = title.find(str);

        if (found != string::npos)
            return true;
    }

    if (field == "authors")
    { // finding str in the field
        found = authors.find(str);

        if (found != string::npos)
            return true;
    }

    if (field == "year")
    { // finding str in the field
        found = year.find(str);

        if (found != string::npos)
            return true;
    }

    if (field == "tags")
    { // finding str in the field
        found = tags.find(str);

        if (found != string::npos)
            return true;
    }
    return false;
}
