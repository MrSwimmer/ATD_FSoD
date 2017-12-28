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

/*bool ex = false;
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
}*/

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    FSD fsd(100);
    unsigned int start_time =  clock();
    for (int i = 0; i < 1000; ++i) {
        //cout << "in" << i << endl;
        fsd.insert(randomStrings(), &i);
    }
    fsd.getall();
    unsigned int end_time = clock();
    int res = end_time - start_time;
    cout << "time " << res << endl;
    char* f;
    gets(f);
    fsd.find(f);
    /*
    FSD fsd(10);
    for(int i=0; i<50; i++) {
        fsd.insert(randomStrings(), &i);
    }*/
    //menu();
    /*set<KP> setblocks;
    for(int i=0; i<100; i++) {
        char *rs = randomStrings();
        cout << rs << endl;
        KP kp(rs, i);
        setblocks.insert(kp);
    }
    cout << endl;
    for(auto i : setblocks) {
        KP kp = i;
        cout << kp.key << endl;
    }

    char *key = "baa";
    int div=2;
    int size=setblocks.size();
    int loc=size/div;
    while(true) {
        int c=0;
        char* keyset;
        for(auto i : setblocks) {
            if(c==loc) {
                keyset = i.key;
                break;
            }
            c++;
        }
        cout << loc;
        if(comparemass(key, keyset)) {
            if(div*2>size) {
                cout << loc-size/div/2-1;
                break;
                //return loc-1;
            } else {
                div*=2;
                loc-=size/div;
            }
        } else {
            if(div>size) {
                cout << loc+size/div/2-1;
                break;
                //return loc+1;
            } else {
                div*=2;
                loc+=size/div;
            }
        }
    }*/
}
