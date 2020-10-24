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

//#include "jstr.h"
#include "jstr/jstr.cpp"

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
    ifstream db_edict("databases/edict_utf8.txt");
    //FILE * db_edict = fopen("edict_utf8.txt", "r");
    //int db_edict = open("edict_utf32.txt", O_RDONLY);
    ofstream output("output.txt");
    ofstream output2("output2.txt");
    string temp, single, copy, temp2, line[256];
    
    getline(db_edict, temp);
    cout << temp << endl;
    for (char glyph : temp) {
        cout << glyph;
    }
    cout << "\n\n";

    getline(db_edict, temp2);
    cout << temp2 << endl;
    for (char glyph : temp2) {
        cout << glyph;
    }
    cout << "\n\n";


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
    //cout << test1 << "\n\n";
    jstr test2 = test1;
    //cout << test2 << "\n\n";
    test2 = temp2;
    //cout << test2 << "\n\n";

    cout << test1[0] << test1[1] << test1[2] << test1[3] << test1[4] << test1[5] << "\n\n";

    jstr test3;
    test3 += test1[0];
    test3 += test1[1];
    test3 += test1[2];
    test3 += test1[3];
    test3 += test1[4];
    test3 += test1[5];

    cout << test3 << "\n\n";

    jstr test4(test3);
    test4 += temp;

    cout << test4 << "\n\n";

    jstr test5(test3);
    test5 += test3;

    cout << test5 << "\n\n";

    jstr test6;
    jstr test7;
    jstr test8;
    jstr test9;
    jstr test10;

    test6 = test3 + test5;
    test7 = test3 + "cat";
    test8 = "cat" + test3;
    test9 = test3 + "C";
    test10 = "C" + test3;

    cout << test6 << "\n";
    cout << test7 << "\n";
    cout << test8 << "\n";
    cout << test9 << "\n";
    cout << test10 << "\n";
    cout << "\n";

    jstr test11(test3);

    cout << (test3 == test11) << endl;
    cout << (test4 == test11) << "\n\n";

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

    db_edict.close();
    output.close();
    output2.close();
    
    return 0;
}