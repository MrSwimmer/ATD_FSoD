//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
using namespace std;

void FSD::insert(string ikey, string inote) {
    ofstream iout(INDEX_FILE_NAME, ios::binary | ios::app);
    ofstream nout(NOTES_FILE_NAME, ios::binary | ios::app);
    //int point = fseek(nout);

    string S = "string";
    nout.write((char*)&S, S.size());
    nout.close();

    KP kp("qqqq", 1);
    iout.write((char*)&kp, sizeof(KP));
    iout.close();
}
