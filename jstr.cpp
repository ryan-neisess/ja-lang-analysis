#pragma once

#include <string>
#include <iostream>
#include <fstream>
using std::string;
using std::ostream;
using std::ofstream;

//    Notes (Simplified)
//
// The jstr class allows the user to treat a Japanese character found in a 
//   UTF-8 encoded string as a single entity, rather than a 3-byte segment 
//   in a std::string that is only printable thanks to modern versions of 
//   the C++ libraries and Linux.
//
// Conceptually, a jstr is a string where each character is a jchar, which 
//   programatically is simply a std::string.

class jstr {
public:
    // Construction of a jstr requires a UTF-8 formatted string, though due 
    //   to the nature of a UTF-8 formatted string, other encodings (such as 
    //   ASCII) can also work

    jstr(const string & src, const int cap); // default constructor, see definition
    jstr(const jstr & src);

    jstr & operator =(const string & src);
    jstr & operator =(const jstr & src);

    void print(ostream & out);
    void print(ofstream & out);

    int size() { return this->size; }
    int len() { return this->size; }
    int capacity() { return this->capacity; }

private:
    string * arr; // pointer for dynamically allocated string
    int size; // number of jchars 
    int capacity; // size of dynamically allocated array

    string * allocate_jstr(int new_cap);

    // friend ostream & operator <<(ostream & lhs, jstr & rhs);
    // friend ofstream & operator <<(ofstream & lhs, jstr & rhs);
};

ostream & operator <<(ostream & lhs, jstr & rhs);
ofstream & operator <<(ofstream & lhs, jstr & rhs);

// * * * * * * * * * Member Function Definitions * * * * * * * * * //

// Default constructor accepting a std::string
// capacity will be the size of the dynamically allocated array of std::strings
jstr::jstr(const string & src = "", const int cap = 64) {
    capacity = cap;
    arr = nullptr;
    *this = src;
}

jstr::jstr(const jstr & src) {
    arr = nullptr;
    *this = src;
}

jstr & jstr::operator =(const string & src) {
    if (arr != nullptr) {
        delete[] arr;
        arr = nullptr;
        size = 0;
        // Note: capacity can be set by the constructor, so don't set to zero
    }

    // The higher of the current capacity and the src's size + 1 will be used
    if (capacity < src.size() + 1) {
        capacity = src.size() + 1;
    }
    if (!allocate_jstr(capacity)) {
        return *this;
    }

    int i = 0;
    string temp;

    while (src[i] != 0) {
        temp = "";

        // UTF-8 encoded chararcters have values >= 0xC0
        if ((unsigned int)src[i] < 0xC0) {
            temp = src[i++];
        }
        else { // if Unicode UTF-8; assumes 3 bytes
            temp.push_back(src[i++]);
            temp.push_back(src[i++]);
            temp.push_back(src[i++]);
        }

        arr[size++] = temp;

        if (size == capacity - 1) { // resize the jstr as needed
            capacity *= 2;
            string * pTemp = arr;
            if (!allocate_jstr(capacity)) {
                return *this;
            }
            for (int j = 0; j <= size; j++) {
                arr[j] = pTemp[j];
            }
            delete[] pTemp;
        }
    }

    arr[size] = ""; // null-character equivalent for jstr

    return *this;
}

jstr & jstr::operator =(const jstr & src) {
    if (arr != nullptr) {
        delete[] arr;
        arr = nullptr;
        capacity = 0;
        size = 0;
    }

    if (!allocate_jstr(src.capacity)) {
        return *this;
    }

    for ( ; src.arr[size] != ""; size++) {
        arr[size] = src.arr[size];
    }
    arr[size] = "";

    return *this;
}

void jstr::print(ostream & out) {
    for (int i = 0; i < size; i++) {
        out << arr[i];
    }
}

void jstr::print(ofstream & out) {
    for (int i = 0; i < size; i++) {
        out << arr[i];
    }
}

string * jstr::allocate_jstr(int new_cap) {
    try {
        arr = new string[new_cap];
        if (arr == nullptr) {
            // will occur if program is linked with nothrownew.obj
            throw 0;
        }
        capacity = new_cap;
        return arr;
    }
    catch (std::bad_alloc & e) {
        std::cout << "Error: bad_alloc exception thrown\n";
        arr = nullptr;
    }
    catch (int e) {
        std::cout << "Error: unable to allocate memory\n";
    }

    // Should only reach here if memory allocation unsuccessful
    capacity = 0;
    size = 0;
    return nullptr;
}

// * * * * * * * * * Non-Member Function Definitions * * * * * * * * * //

ostream & operator <<(ostream & lhs, jstr & rhs) {
    rhs.print(lhs);
    return lhs;
}

ofstream & operator <<(ofstream & lhs, jstr & rhs) {
    rhs.print(lhs);
    return lhs;
}



//     Notes (Verbose)
//
// The jstr class is designed to provide a convenient way to work with UTF-8 
//   encoded strings such that each human-readable character has an equivalent 
//   computer readable "character", while still maintaining some degree of 
//   efficiency.
//
// The term "jchar" refers to a single human-readable character, whether it is 
//   a more "common/simple" Latin or similar character, or, a more "complex" 
//   character such as a Japanese character. Effectively, the jstr class 
//   represents a string of jchars in order to allow a single English or 
//   Japanese character to be treated as a single character.
//
// The need for such arises because under UTF-8 formatting, typical ASCII 
//   chars are represented by a singly byte, whereas other less common/simple 
//   chars are represented by three bytes, which is the size needed to contain 
//   such chars. While modern C++ programs running in modern Linux 
//   environments can deal with an entire UTF-encoded string, even those 
//   containing Japanese characters, with ease for operations such as printing, 
//   there is no easy method to access a single human-readable character.
//
// As a result, the jstr class keeps track of each human-readable character as 
//   its own string, so that each individual string (representing a jchar) can 
//   take advantage of the built in (and undoubtedly complex) internal methods 
//   for dealing with UTF-8 encoded strings, while still allowing the user the 
//   ability to work with each human-readable character as a stand-alone char.
//
// The motivation behind the jstr class arose from my own desire to work with 
//   Japanese language databases on a character by character basis and finding 
//   no convenient means of doing so after reasonably extensive research into 
//   working with Unicode characters with C/C++. With the nature of each jchar 
//   represented programatically as a std::string, there was deemed to be no 
//   need to create a separate jchar class as 
//
// Due to each jchar being represented by a std::string, and all operations 
//   needed being those that already exist for the std::string class, it was 
//   deemed unnecessary to create a separate jchar class, and to instead 
//   simply work with the understanding that a jchar was a theoretical unit 
//   implemented fully by a std::string, with more complex operations taken 
//   care of in the jstr class.
