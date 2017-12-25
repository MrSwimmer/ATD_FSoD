#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
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
    int a=100;
    FSD fsd(a);
    //fsd.getall();
    for(int i=0; i<101; i++){
        char *m = randomStrings();
        //cout << m << endl;
        fsd.insert(m, &i);
        //fsd.getall();
    }
    fsd.getall();
    getchar();
    /*int a =3;
    fsd.insert(const_cast<char *>("key"), &a);*/

    return 0;
}
