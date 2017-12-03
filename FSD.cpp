//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
#include "Note.h"
#include <iostream>

using namespace std;

void FSD::insert(string ikey, Note iNote) {

    ofstream iout(INDEX_FILE_NAME, ios::binary | ios::app);
    ofstream nout(NOTES_FILE_NAME, ios::binary | ios::app);
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    ifstream nin(NOTES_FILE_NAME, ios::binary);
    fstream file(NOTES_FILE_NAME);

    long long point;
    file.seekg (0, ios::end);
    point = file.tellg();
    nout.write((char*)&iNote, sizeof(Note));
    nout.close();

    KP kp(ikey, point);
    iout.write((char*)&kp, sizeof(KP));
    iout.close();

    KP kpr;
    while (!iin.eof()){
        iin.read((char*)&kpr, sizeof(KP));
        cout << kpr.key << " " << kpr.pointer << endl;
    }
    while (!nin.eof()){
        Note note;
        nin.read((char*)&note, sizeof(Note));
        cout << note.note << endl;
    }
    iin.close();
}


