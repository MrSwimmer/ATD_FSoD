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
    FSD fsd;
    for(int i=0; i<10000; i++) {
        char *str = randomStrings();
        fsd.insert(str, &i);
    }
    fsd.getNote(const_cast<char *>("key"));
    fsd.delNote(const_cast<char *>("kew"));
    list<char*> mylist;
    fsd.getall(mylist);
    return 0;
}
