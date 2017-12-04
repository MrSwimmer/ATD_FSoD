//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
#include <iostream>

using namespace std;

void FSD::insert(char* ikey, void* iNote, int size) {

    ofstream iout(INDEX_FILE_NAME, ios::binary | ios::app);
    ofstream nout(NOTES_FILE_NAME, ios::binary | ios::app);

    fstream file(NOTES_FILE_NAME);

    long long point;
    file.seekg (0, ios::end);
    point = file.tellg();

    nout.write((char*)&size, sizeof(int));
    nout.write((char*)iNote, size);
    nout.close();

    KP kp(ikey, point);
    iout.write((char*)&kp, sizeof(KP));
    iout.close();
}

void FSD::getnote(char *dest, char* key) {
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    ifstream nin(NOTES_FILE_NAME, ios::binary);
    KP kpr;
    while (!iin.eof()){
        iin.read((char*)&kpr, sizeof(KP));
        cout << key << " " << kpr.key;
        if(kpr.key==key){
            nin.seekg(kpr.pointer);
            int ln;
            iin.read((char*)&ln, sizeof(int));
            dest = new char[ln];
            nin.read((char*)dest, sizeof(int));
            cout << "OK" << endl;
            break;
        }
        //cout << kpr.key << " " << kpr.pointer << endl;
    }
    iin.close();
}


