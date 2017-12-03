#include <iostream>
#include <fstream>
#include "KP.h"
#include "FSD.h"

using namespace std;
int main() {
    FSD fsd;
    //INote<int> iNote;
    Note note("oooooo");
    fsd.insert("key", note);
    return 0;
}
