#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "myCW/KP.h"
#include "myCW/FSD.h"
using namespace std;


int main() {
    FSD fsd;
    char k[] = "qqqqqqq";
    fsd.insert("key", k, 7);
    return 0;
}