#include <iostream>
#include <fstream>
#include "KP.h"
#include "FSD.h"

using namespace std;
int main() {
    FSD fsd;
    char note[] = "iiiiiii";
    fsd.insert("key", &note, sizeof(note));
    fsd.insert("keq", &note, sizeof(note));
    fsd.insert("kew", &note, sizeof(note));
    fsd.insert("kee", &note, sizeof(note));
    fsd.insert("ker", &note, sizeof(note));
    char get[] = {};
    fsd.getnote(get, "kew");
    return 0;
}
