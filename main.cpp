#include <iostream>
#include <cstdlib>
#include <ctime>
#include "FSD.h"

using namespace std;
int main() {

    FSD fsd;
    char *note= "iiiiiii";
    //fsd.fillInAll();
    fsd.insert("key", note, sizeof(note));
    /*fsd.insert("keq", &note, sizeof(note));
    fsd.insert("kew", &note, sizeof(note));
    fsd.insert("kee", &note, sizeof(note));
    fsd.insert("ker", &note, sizeof(note));*/
    /*char get[] = {};
    fsd.getnote(get, "kew");*/
    fsd.getall();
    return 0;
}
