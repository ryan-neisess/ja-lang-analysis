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

bool operator ==(const jchar & lhs, const char * & rhs);
bool operator ==(const char * & lhs, const jchar & rhs);
bool operator ==(const jchar & lhs, const string & rhs);
bool operator ==(const string & lhs, const jchar & rhs);
bool operator ==(const jchar & lhs, const jchar & rhs);

bool operator !=(const jchar & lhs, const char * & rhs);
bool operator !=(const char * & lhs, const jchar & rhs);
bool operator !=(const jchar & lhs, const string & rhs);
bool operator !=(const string & lhs, const jchar & rhs);
bool operator !=(const jchar & lhs, const jchar & rhs);

// Precondition: sym represents ONLY a SINGLE human-readable character in UTF-8
jchar::jchar(const string & sym = "") {
    symbol = sym;
}

jchar::jchar(const jchar & sym) {
    *this = sym;
}

// Precondition: sym represents ONLY a SINGLE human-readable character in UTF-8
jchar & jchar::operator =(const string & sym) {
    symbol = sym;
    return *this;
}

jchar & jchar::operator =(const jchar & sym) {
    symbol = sym.symbol;
    return *this;
}

ostream & operator <<(ostream & lhs, const jchar & rhs) {
    lhs << rhs.symbol;
    return lhs;
}

// See notes for details
// ofstream & operator <<(ofstream & lhs, const jchar & rhs) {
//     lhs << rhs.symbol;
//     return lhs;
// }

// Boolean == operator overloads

bool operator ==(const jchar & lhs, const char * & rhs) {
    return lhs.symbol == rhs;
}

bool operator ==(const char * & lhs, const jchar & rhs) {
    return lhs == rhs.symbol;
}

bool operator ==(const jchar & lhs, const string & rhs) {
    return lhs.symbol == rhs;
}

bool operator ==(const string & lhs, const jchar & rhs) {
    return lhs == rhs.symbol;
}

bool operator ==(const jchar & lhs, const jchar & rhs) {
    return lhs.symbol == rhs.symbol;
}

// Boolean != operator overloads
// return !(lhs == rhs); syntax not chosen in order to minimize function calls/
//   stack frames

bool operator !=(const jchar & lhs, const char * & rhs) {
    return lhs.symbol != rhs;
}

bool operator !=(const char * & lhs, const jchar & rhs) {
    return lhs != rhs.symbol;
}

bool operator !=(const jchar & lhs, const string & rhs) {
    return lhs.symbol != rhs;
}

bool operator !=(const string & lhs, const jchar & rhs) {
    return lhs != rhs.symbol;
}

bool operator !=(const jchar & lhs, const jchar & rhs) {
    return lhs.symbol != rhs.symbol;
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
jstr operator +(const jstr & lhs, const jchar & rhs);
jstr operator +(const string & lhs, const jstr & rhs);
jstr operator +(const jstr & lhs, const string & rhs);
jstr operator +(const jstr & lhs, const jstr & rhs);

bool operator ==(const jstr & lhs, const jstr & rhs);
bool operator !=(const jstr & lhs, const jstr & rhs);

// * * * * * * jstr public Member Function Definitions * * * * * * //

// Default constructor accepting a std::string
// capacity will be the size of the dynamically allocated array of std::strings
jstr::jstr(const string & src = "", const int cap = 64) {
    _capacity = cap;
    _size = 0;
    arr = nullptr;
    *this = src;
}

jstr::jstr(const jstr & src) {
    _capacity = 0;
    _size = 0;
    arr = nullptr;
    *this = src;
}

jstr::~jstr() {
    clear(0);
}

jstr & jstr::operator =(const string & src) {
    if (arr != nullptr) {
        delete[] arr;
        arr = nullptr;
        _size = 0;
        // Note: capacity can be set by the constructor, so don't set to zero
    }

    // The higher of the current capacity and the src's size + 1 will be used
    if (_capacity < src.size() + 1) {
        _capacity = src.size() + 1;
    }
    if (!allocate_jstr(_capacity)) {
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

        arr[_size++] = temp;

        if (_size == _capacity - 1) { // resize the jstr as needed
            _capacity *= 2;
            jchar * pTemp = arr;
            if (!allocate_jstr(_capacity)) {
                return *this;
            }
            for (int j = 0; j <= _size; j++) {
                arr[j] = pTemp[j];
            }
            delete[] pTemp;
        }
    }

    arr[_size] = ""; // null-character equivalent for jstr

    return *this;
}

jstr & jstr::operator =(const jstr & src) {
    if (arr != nullptr) {
        delete[] arr;
        arr = nullptr;
        _capacity = 0;
        _size = 0;
    }

    if (!allocate_jstr(src._capacity)) {
        return *this;
    }

    for ( ; src.arr[_size] != ""; _size++) {
        arr[_size] = src.arr[_size];
    }
    arr[_size] = "";

    return *this;
}

jstr & jstr::operator +=(const jchar & to_add) {
    if (_capacity < _size + 2){ // extra +1 for new jchar
        _capacity <<= 2;

        if (!allocate_jstr(_capacity)) {
            return *this;
        }
    }

    arr[_size++] = to_add;
    return *this;
}

jstr & jstr::operator +=(const string & to_add) {
    jstr converted(to_add);
    *this += converted;
    return *this;
}

jstr & jstr::operator +=(const jstr & to_add) {
    int size_needed = _size + 1 + to_add._size;

    if (_capacity < size_needed) {
        do {
            _capacity <<= 2;
        } while (_capacity < size_needed);

        if (!allocate_jstr(_capacity)) {
            return *this;
        }
    }

    for (int i = 0; i < to_add._size; i++) {
        arr[_size++] = to_add.arr[i];
    }
    arr[_size++] = ""; // null-character equivalent
    return *this;
}

void jstr::print(ostream & out) const {
    for (int i = 0; i < _size; i++) {
        out << arr[i];
    }
}

void jstr::print(ofstream & out) const {
    for (int i = 0; i < _size; i++) {
        out << arr[i];
    }
}

// If keep == 1, former capacity is retained and a logical clear/delete is 
//   performed; if keep == 0, delete[] is called and capacity is set to 0
void jstr::clear(int keep = 1) {
    if (keep == 1) {
        arr[0] = "";
    }
    else {
        delete[] arr;
        arr = nullptr;
        _capacity = 0;
    }

    _size = 0;
}

// * * * * * * jstr private Member Function Definitions * * * * * * //

jchar * jstr::allocate_jstr(const int new_cap) {
    try {
        arr = new jchar[new_cap];
        if (arr == nullptr) {
            // will occur if program is linked with nothrownew.obj
            throw 0;
        }
        _capacity = new_cap;
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
    _capacity = 0;
    _size = 0;
    return nullptr;
}

// * * * * * * jstr Non-Member Function Definitions * * * * * * //

ostream & operator <<(ostream & lhs, const jstr & rhs) {
    rhs.print(lhs);
    return lhs;
}

// See notes for details
// ofstream & operator <<(ofstream & lhs, const jstr & rhs) {
//     rhs.print(lhs);
//     return lhs;
// }

jstr operator +(const jchar & lhs, const jstr & rhs) {
    jstr result(lhs.symbol);
    result += rhs;
    return result;
}

jstr operator +(const jstr & lhs, const jchar & rhs) {
    jstr result(lhs);
    result += rhs;
    return result;
}

jstr operator +(const string & lhs, const jstr & rhs) {
    jstr result(lhs);
    result += rhs;
    return result;
}

jstr operator +(const jstr & lhs, const string & rhs) {
    jstr result(lhs);
    result += rhs;
    return result;
}

jstr operator +(const jstr & lhs, const jstr & rhs) {
    jstr result(lhs);
    result += rhs;
    return result;
}

bool operator ==(const jstr & lhs, const jstr & rhs) {
    //return jstrcmp(lhs, rhs) == 0; // maybe implement later

    int i = 0;
    for ( ; lhs[i] != "" && rhs[i] != ""; i++) { }
    if (lhs[i] == rhs[i]) { return true; }
    else { return false; }
}

bool operator !=(const jstr & lhs, const jstr & rhs) {
    return !(lhs == rhs);
}

// Need to consider how to compare jchars
// int jstrcmp(const jstr & jstr1, const jstr & jstr2) {
//     for (int i = 0; jstr1[i] != "" || jstr2[i] != ""; i++) {
//         if (jstr1[i] > jstr2[i]) return 1;
//         else if (jstr1[i] < jstr1[i]) return -1;
//     }
// }



//     Notes (Verbose) - To Be Revised
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
