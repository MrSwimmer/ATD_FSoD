#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "FSD.h"

using namespace std;

char *randomStrings(int sybols = 3) {
    char *str;
    str = new char[sybols + 1];
    for (int i = 0; i < sybols; i++) {
        str[i] = static_cast<char>(97 + rand() % 10);
    }
    str[sybols] = '\0';
    return str;
}

bool ex = false;
FSD fsd(10);

char *key;

void menu() {
    int k = 1;
    while (k != 0) {
        cout << "1.Insert" << endl;
        cout << "2.Delete" << endl;
        cout << "3.Get" << endl;
        cout << "4.Get all" << endl;
        cin >> k;
        switch (k) {
            case 1:
                for(int i=0; i<100; i++) {
                    fsd.insert(randomStrings(), &i);
                }
                fsd.getall();
                break;
            case 2:
                cout << "Please enter key" << endl;
                gets(key);
                fsd.delNote(key);
                cout << "__ready__" << endl;
                break;
            case 3:
                cout << "Please enter key" << endl;
                gets(key);
                cout << fsd.getNote(key) << endl;
                cout << "__ready__" << endl;
                break;
            case 4:
                fsd.getall();
                cout << endl << "__ready__" << endl;
                break;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    menu();
    return 0;
}
