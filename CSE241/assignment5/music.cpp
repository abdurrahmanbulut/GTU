#include <iostream>
#include "catalog_movie_music_book.h"

using namespace std;

// Default constructor
Music::Music()
{ 
    title = "";
    artists = "";
    year = "";
    genre = "";
}
// Constructor that store member datas with Line infos.
Music::Music(const string& line) {

    int quetosIndexes[8];// keeps double quetos index in the data line

    int indexCounter = 0;

    for (int j = 0; j < line.length(); j++) {// finding indexes from data line

        if (line[j] == '"') 
            quetosIndexes[indexCounter++] = static_cast<int>(j);
    }

    // getting pieces from data line
    title = line.substr(quetosIndexes[0] + 1, quetosIndexes[1] - quetosIndexes[0] - 1);
    artists = line.substr(quetosIndexes[2] + 1, quetosIndexes[3] - quetosIndexes[2] - 1);
    year = line.substr(quetosIndexes[4] + 1, quetosIndexes[5] - quetosIndexes[4] - 1);
    genre = line.substr(quetosIndexes[6] + 1, quetosIndexes[7] - quetosIndexes[6] - 1);

}
// << operator overloading. Prints movie object.
ostream &operator<< (ostream& term, const Music& music) {

    term << '"' << music.getTitle() << "\" \"" << music.getArtists() << "\" \"" << music.getYear() << "\" \"" 
                 << music.getGenre() << '"' << endl;

    return term;
}
// getter for title name
const string& Music::getTitle() const 
{
    return title;
}
// getter for artists name
const string& Music::getArtists() const 
{
    return artists;
}
// getter for year
const string& Music::getYear() const 
{
    return year;
}
// getter for genre
const string& Music::getGenre() const 
{
    return genre;
}
// It searchs str in fieldç If it finds, returns true. 
bool Music::search(const string &str, const string &field) {

    int found;

    if(field == "title")
    { 
        found = title.find(str);

        if(found != string::npos)
            return true;
    }

    if(field == "artists")
    { 
        found = artists.find(str);

        if(found != string::npos)
            return true;
    }

    if(field == "year")
    {
        found = year.find(str);

        if(found != string::npos)
            return true;
    }
    // finding str in the field
    if(field == "genre")
    { 
        found = genre.find(str);

        if(found != string::npos)
            return true;
    }
    return false;
}
