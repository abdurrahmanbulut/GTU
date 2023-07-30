// Abdurrahman BULUT
//========================================
// Movie.cpp implementation file
// This file contains Movie class implementations

#include <iostream>
#include "catalog_movie_music_book.h"
#include <string>

// Default constructor
Movie::Movie()
{
    title = "";
    director = "";
    year = "";
    genre = "";
    starring = "";
}
// Constructor that store member datas with Line infos.
Movie::Movie(const string& Line)
{

    int counter = 0, indexes[10]; // keeps double quetos index in the data line

    for (int i = 0; i < Line.length(); i++) // finding indexes from data line
    {
        if (Line[i] == '"')
            indexes[counter++] = static_cast<int>(i);
    }

    director = Line.substr(indexes[2] + 1, indexes[3] - indexes[2] - 1);
    title = Line.substr(indexes[0] + 1, indexes[1] - indexes[0] - 1);
    genre = Line.substr(indexes[6] + 1, indexes[7] - indexes[6] - 1);
    starring = Line.substr(indexes[8] + 1, indexes[9] - indexes[8] - 1);
    year = Line.substr(indexes[4] + 1, indexes[5] - indexes[4] - 1);
}
// getter for title name
const string& Movie::getTitle() const
{
    return title;
}
// getter for genre
const string& Movie::getGenre() const
{
    return genre;
}
// getter for starring
const string& Movie::getStarring() const
{
    return starring;
}
// getter for year
const string& Movie::getYear() const
{
    return year;
}
// getter for director
const string& Movie::getDirector() const
{
    return director;
}
// << operator overloading. Prints movie object.
ostream& operator<<(ostream& outputStream, const Movie& movie)
{
    outputStream << '"' << movie.getTitle() << "\" \"" << movie.getDirector() << "\" \"" << movie.getYear()
                 << "\" \"" << movie.getGenre() << "\" \"" << movie.getStarring() << '"' << endl;

    return outputStream;
}
// It searchs str in fieldç If it finds, returns true. 
bool Movie::search(const string& str, const string& field)
{

    int found;
    
    // str in the field
    if (field == "title")
    { 
        found = title.find(str);

        if (found != string::npos)
            return true;
    }

    if (field == "director")
    { 
        found = director.find(str);

        if (found != string::npos)
            return true;
    }

    if (field == "year")
    { 
        found = year.find(str);

        if (found != string::npos)
            return true;
    }

    if (field == "genre")
    { 
        found = genre.find(str);

        if (found != string::npos)
            return true;
    }
    
    if (field == "starring")
    { 
        found = starring.find(str);

        if (found != string::npos)
            return true;
    }
    return false;
}
