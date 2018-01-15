#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "FSD.h"

using namespace std;
bool comparemass(char *a, char *b) {
    if(b=="_") {
        return true;
    } else {
        for(int i=0; a[0]!='\0'; i++) {
            if(a[i]<b[i]) {
                return true;
            } else if (a[i]>b[i]) {
                return false;
            }
        }
        return true;
    }
}
char *randomStrings(int sybols = 3) {
    char *str;
    str = new char[sybols + 1];
    for (int i = 0; i < sybols; i++) {
        str[i] = static_cast<char>(97 + rand() % 10);
    }
    str[sybols] = '\0';
    return str;
}

void menu(FSD fsd) {
    int k = 1;
    int d;
    char key[20];
    unsigned int start_time, end_time;
    int res;
    while (k != 0) {
        cout << "1.Insert" << endl;
        cout << "2.Delete" << endl;
        cout << "3.Get" << endl;
        cout << "4.Insert all" << endl;
        cin >> k;
        switch (k) {
            case 1:
                cout << "Please enter data" << endl;
                cin >> d;
                fsd.insert(randomStrings(), &d);
                break;
            case 2:
                cout << "Please enter key" << endl;
                cin >> key;
                fsd.del(key);
                cout << "__ready__" << endl;
                break;
            case 3:
                cout << "Please enter key" << endl;
                cin >> key;
                start_time =  clock();
                fsd.find(key);
                end_time = clock();
                res = end_time - start_time;
                cout << "time " << res << endl;
                cout << "__ready__" << endl;
                break;
            case 4:
                int n;
                cout << "Please enter amount " << endl;
                cin >> n;
                start_time =  clock();
                for (int i = 0; i < n; ++i) {
                    fsd.insert(randomStrings(), &i);
                }
                end_time = clock();
                res = end_time - start_time;
                cout << "time " << res << endl;
            case 5:
                fsd.getall();
                break;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    cout << "enter size of block" << endl;
    int n;
    cin >> n;
    FSD fsd(n);
    menu(fsd);
}
