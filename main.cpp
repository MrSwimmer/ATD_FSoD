#include <iostream>
#include <cstdlib>
#include <ctime>
#include "FSD.h"

using namespace std;
int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    FSD fsd;
    char note[] = "iiiiiii";
    fsd.insert("key", &note, sizeof(note));
    fsd.insert("keq", &note, sizeof(note));
    fsd.insert("kew", &note, sizeof(note));
    fsd.insert("kee", &note, sizeof(note));
    fsd.insert("ker", &note, sizeof(note));
    char get[] = {};
    fsd.getnote(get, "kew");
    fsd.getall();
    return 0;
}
