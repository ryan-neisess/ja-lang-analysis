#pragma once

#include <iostream>
#include <string>
using std::string;

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

    jstr(const string & src, const int max);

    jstr(const jstr & src);

    jstr & operator=(const string & src);
    jstr & operator=(const jstr & src);

//private:
    string * arr; // pointer for dynamically allocated string
    int size; // number of jchars 
    int max_size; // size of dynamically allocated array
};

// overloaded << operators

// * * * * * * * * * Function Definitions * * * * * * * * * //

// Default constructor accepting a std::string
// max_size will be the size of the dynamically allocated array of std::strings
jstr::jstr(const string & src = "", const int max = 64) {
    max_size = max;
    *this = src;
}

jstr::jstr(const jstr & src) {
    *this = src;
}

jstr & jstr::operator=(const string & src) {
    int i = 0;
    string temp;

    arr = new string[max_size];

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

        if (size == 63) {
            std::cout << "Warning: src string too long, truncating\n";
            break;
        }
    }

    arr[size] = "";

    return *this;
}

jstr & jstr::operator=(const jstr & src) {
    // check that this is empty first

    try {
        arr = new string[src.max_size];
        if (arr == nullptr) {
            // will occur if program is linked with nothrownew.obj
            throw 0;
        }
        max_size = src.max_size;
    }
    catch (std::bad_alloc & e) {
        std::cout << "Error: unable to allocate memory\n";
    }
    catch (int e) {
        std::cout << "Error: unable to allocate memory\n";
    }

    for ( ; src.arr[size] != ""; size++) {
        arr[size] = src.arr[size];
    }
    arr[size] = "";

    return *this;
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
