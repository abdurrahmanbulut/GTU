// Abdurrahman BULUT
//========================================
// catalog_movie_music_book class definitions

#ifndef CLASSES_H_BULUT
#define CLASSES_H_BULUT

#include <vector>
#include <string>
#include "exceptions.h"

using namespace std;

//====================================================
class Book {

public:
    Book(); // default constructor
    Book(const string& line); // other constructor. It gets line from data.
    const string& getTitle() const; // title name getter
    const string& getAuthors() const; // author name getter
    const string& getYear() const; // year value getter
    const string& getTags() const; // tag names getter
    bool search(const string &str, const string &field); // searches str in field
    friend ostream& operator<<(ostream& outputStream, const Book& book); // overloading redirecting operator

private:
    string title;
    string authors;
    string year;
    string tags;
};
//====================================================
class Movie {

public:

    Movie();// default constructor
    Movie(const string &line); // other constructor. It gets line from data.
    const string& getTitle() const;  // title name getter
    const string& getGenre() const; // Genre getter
    const string& getStarring() const;  // starring getter
    const string& getYear() const; // year value getter
    const string& getDirector() const; // Director name getter
    bool search(const string& str, const string& field);// searches str in field
    friend ostream &operator<<(ostream& outputStream, const Movie& movie);// operator overloading

private:
    string director;
    string title;
    string genre;
    string starring;    
    string year;
};
//====================================================
class Music {

public:

    Music();// default constructor
    Music(const string& line); // other constructor. It gets line from data.
    const string& getTitle() const; // title name getter
    const string& getArtists() const; // artist name getter
    const string& getYear() const; // year getter
    const string& getGenre() const; // // genre  getter
    bool search(const string& str, const string& field);// searches str in field
    friend ostream &operator<< (ostream& term, const Music& music);// operator overloading

private:
    string title;
    string artists;
    string year;
    string genre;
};

//====================================================
template<class T>
class Catalog { // template Catalog class for keeping book, music and movie classes

public:

    Catalog(); // default constructor
    Catalog(const string &catalogName); // taking catalog name constructor
    void getDataFromFile(); // gets line from data
    void printData(); // prints data
    void printUniqueEntry(); // prints number of unique entry to output file
    void push_back(T item); // takes item and adds end of the data
    int size(); // return size
    vector<T>& getData(); // getter for data
    T &operator[](int index); // index operator overloading

    //inline functs
    // compare functions for using sort function in main
    static bool compareBookTitle(const Book &lhs, const Book &rhs) { return lhs.getTitle() < rhs.getTitle(); }
    static bool compareBookAuthors(const Book &lhs, const Book &rhs) { return lhs.getAuthors() < rhs.getAuthors(); }
    static bool compareBookYear(const Book &lhs, const Book &rhs) { return lhs.getYear() < rhs.getYear(); }
    static bool compareBookTags(const Book &lhs, const Book &rhs) { return lhs.getTags() < rhs.getTags(); }
    static bool compareMusicTitle(const Music &lhs, const Music &rhs) { return lhs.getTitle() < rhs.getTitle(); }
    static bool compareMusicArtists(const Music &lhs, const Music &rhs) { return lhs.getArtists() < rhs.getArtists(); }
    static bool compareMusicYear(const Music &lhs, const Music &rhs) { return lhs.getYear() < rhs.getYear(); }
    static bool compareMusicGenre(const Music &lhs, const Music &rhs) { return lhs.getGenre() < rhs.getGenre(); }
    static bool compareMovieYear(const Movie &lhs, const Movie &rhs) { return lhs.getYear() < rhs.getYear(); }
    static bool compareMovieTitle(const Movie &lhs, const Movie &rhs) { return lhs.getTitle() < rhs.getTitle(); }
    static bool compareMovieDirector(const Movie &lhs, const Movie &rhs) { return lhs.getDirector() < rhs.getDirector(); }
    static bool compareMovieGenre(const Movie &lhs, const Movie &rhs) { return lhs.getGenre() < rhs.getGenre(); }
    static bool compareMovieStarring(const Movie &lhs, const Movie &rhs) { return lhs.getStarring() < rhs.getStarring(); }

private:
    vector<T> data;     // data vector
    int vectorSize;
    string catalogName;
};

#endif
