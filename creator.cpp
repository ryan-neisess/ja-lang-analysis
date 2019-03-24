#include <iostream>
#include <fstream>
#include <string>

//#include <uchar.h>
#include <cuchar>
#include <cstdio>
#include "unistd.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"

#include <cstring>

#include "jstr.cpp"

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::getline;

//using std::u32string;

int main(void) {
    ifstream db_edict("edict_utf8.txt");
    //FILE * db_edict = fopen("edict_utf8.txt", "r");
    //int db_edict = open("edict_utf32.txt", O_RDONLY);
    ofstream output("output.txt");
    ofstream output2("output2.txt");
    string temp, single, copy, temp2, line[256];
    
    getline(db_edict, temp);
    
    cout << temp << endl << endl;
    int x = 0;

    for (char glyph : temp) {
        cout << glyph;
        x++;
    }

    cout << "\n\nx: " << x << "\n\n";

    /*int i = 0, size = 0;

    while (temp[i] != 0) {
        temp2 = "";
        if ((unsigned int)temp[i] < 0xC0) {
            temp2 = temp[i++];
            line[size++] = temp2;
            cout << temp2 << endl;
        }
        else {
            //std::copy(&temp[i], &temp[i + 3], &temp2[0]);
            //memcpy(&temp2[0], &temp[i], 3);
            temp2.push_back(temp[i++]);
            temp2.push_back(temp[i++]);
            temp2.push_back(temp[i++]);
            //i += 3;
            line[size++] = temp2;
            cout << temp2 << "_JA" << endl;
        }
        //cout << temp2;
    }

    for (int j = 0; j < size; j++) {
        cout << line[j];
    }

    cout << "\n\nsize: " << size << "\n\n";*/

    jstr test1 = temp;

    for (int i = 0; i < test1.size; i++) {
        cout << test1.arr[i];
    }

    cout << "\n\n";

    // char16_t ctemp;
    // int i = 0;
    // temp2 = " ";
    // char buf[1024];
    // read(db_edict, buf, 1024);
    // unsigned int * pBuf = (unsigned int *)buf;

    // while(i < 64) {
    //     unsigned int rtemp = 0;
    //     rtemp = *(pBuf + 1);
    //     *(pBuf + 1) = 0;
    //     printf("%s\n", (char *)pBuf);
    //     *(pBuf + 1) = rtemp;
    //     pBuf++;
    //     i++;
    // }

    
    return 0;
}