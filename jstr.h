#pragma once

#include <string>
#include <iostream>
#include <fstream>
using std::string;
using std::ostream;
using std::ofstream;

//    Notes (Simplified)
//
// The jstr class is a logical string class where every character in a jstr 
//   is of type jchar. The jstr class allows a UTF-8 formatted Unicode string 
//   to be stored and manipulated in such a way that each human-readable 
//   character can also be treated programatically as a single jchar.
//
// The jstr class was created due to limitations in working with Unicode 
//   characters, namely that UTF-8 encoding does not enforce fixed-width 
//   characters, likely in an effort to save space (e.g. Latin and similar 
//   characters are 1 byte, while Japanese and similar characters are 3 bytes).
//
// Currently, the jstr class is compatible only with modern versions of C++/ 
//   C++ compilers in a Linux environment, due to the internal methods in the 
//   std::string class that allow for printing of UTF-8 encoded characters.

class jchar {
public:
    jchar(const string & sym);
    jchar(const jchar & sym);

    jchar & operator =(const string & sym);
    jchar & operator =(const jchar & sym);

    string symbol;

    // Note: symbol was decided to be public because the jchar class is 
    //   effectively just a wrapper class for a string, providing the 
    //   distinction and clarity however that said string is for representing 
    //   a single human-readable character, as opposed to an entire UTF-8 
    //   formatted string containing multiple human-readable characters
};

ostream & operator <<(ostream & lhs, const jchar & rhs);
//ofstream & operator <<(ofstream & lhs, const jchar & rhs);

template <typename HRC>
bool operator ==(const jchar & lhs, const HRC & rhs);

template <typename HRC>
bool operator ==(const HRC & lhs, const jchar & rhs);

bool operator ==(const jchar & lhs, const jchar & rhs);

template <typename HRC>
bool operator !=(const jchar & lhs, const HRC & rhs);

template <typename HRC>
bool operator !=(const HRC & lhs, const jchar & rhs);

bool operator !=(const jchar & lhs, const jchar & rhs);

// * * * * * * jchar Templated Function Definitions * * * * * * //

// Acceptable types for HRC include char * and std::string
template <typename HRC>
bool operator ==(const jchar & lhs, const HRC & rhs) {
    return lhs.symbol == rhs;
}

// Acceptable types for HRC include char * and std::string
template <typename HRC>
bool operator ==(const HRC & lhs, const jchar & rhs) {
    return lhs == rhs.symbol;
}

// Acceptable types for HRC include char * and std::string
template <typename HRC>
bool operator !=(const jchar & lhs, const HRC & rhs) {
    return lhs.symbol != rhs;
}

// Acceptable types for HRC include char * and std::string
template <typename HRC>
bool operator !=(const HRC & lhs, const jchar & rhs) {
    return lhs != rhs.symbol;
}



class jstr {
public:
    // Construction of a jstr requires a UTF-8 formatted string, though due 
    //   to the nature of a UTF-8 formatted string, other encodings (such as 
    //   ASCII) can also work

    jstr(const string & src, const int cap); // default constructor, see definition
    jstr(const jstr & src);

    ~jstr();

    // Big-5, In Development

    jstr & operator =(const string & src);
    jstr & operator =(const jstr & src);

    jchar & operator [](int index) { return arr[index]; }
    const jchar & operator [](int index) const { return arr[index]; }

    jstr & operator +=(const jchar & to_add);
    jstr & operator +=(const string & to_add);
    jstr & operator +=(const jstr & to_add);

    // Acceptable types for J include jstr, std::string, and jchar
    template <typename J>
    jstr & append(J & to_add) { return *this += to_add; }

    void print(ostream & out) const;
    void print(ofstream & out) const;

    void clear(int keep); // keep default is 1

    int size() const { return _size; }
    int len() const { return _size; }
    int capacity() const { return _capacity; }

private:
    jchar * arr; // pointer for dynamically allocated string of jchars
    int _size; // number of jchars 
    int _capacity; // size of dynamically allocated array

    jchar * allocate_jstr(int new_cap);
};

ostream & operator <<(ostream & lhs, const jstr & rhs);
//ofstream & operator <<(ofstream & lhs, const jstr & rhs);

jstr operator +(const jchar & lhs, const jstr & rhs);
jstr operator +(const string & lhs, const jstr & rhs);

template <typename J>
jstr operator +(const jstr & lhs, const J & rhs);

bool operator ==(const jstr & lhs, const jstr & rhs);
bool operator !=(const jstr & lhs, const jstr & rhs);

// * * * * * * jstr Templated Function Definitions * * * * * * //

// Acceptable types for J include jstr, std::string, and jchar
template <typename J>
jstr operator +(const jstr & lhs, const J & rhs) {
    jstr result(lhs);
    result += rhs;
    return result;
}