#include <iostream>
#include <cstdlib>
#include <ctime>
#include "FSD.h"

using namespace std;
char * randomStrings (int sybols = 3) {
    char *str;
    str = new char [sybols+1];
    for (int i = 0; i < sybols; i++)
    {
        str [i]  = static_cast<char>(97 + rand() % 10);
    }
    str[sybols] = '\0';
    return str;
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    /*char *a = randomStrings();
    char *b = randomStrings();
    cout << a << " " << b << endl;
    if(comparemas(a,b)) {
        cout << "yes";
    } else {
        cout << "no";
    }*/
    FSD fsd;
    //fsd.getall();
    char *note= "iiiiiii";
    fsd.insert("key", note);
    fsd.getall();
    /*for(int i=0; i<1000; i++) {
        char *str = randomStrings();
        fsd.insert(str, &i, sizeof(str));
    }*/
    //char *note= "iiiiiii";
    //fsd.fillInAll();

    /*fsd.insert("keq", &note, sizeof(note));
    fsd.insert("kew", &note, sizeof(note));
    fsd.insert("kee", &note, sizeof(note));
    fsd.insert("ker", &note, sizeof(note));*/
    /*char get[] = {};
    fsd.getnote(get, "kew");*/

    return 0;
}
