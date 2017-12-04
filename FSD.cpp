//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
#include "IndexInfo.h"
#include <iostream>
#include <io.h>

bool FileExists(const char *fname)
{
    return access(fname, 0) != -1;
}
using namespace std;
FSD::FSD() {
    if(FileExists(INDEX_FILE_NAME)){
        cout << "YUP";
    } else {
        cout << "NOPE";
        ofstream iout(INDEX_FILE_NAME, ios::binary);
        IndexInfo cblocks("COUNT_BLOCKS", COUNT_BLOCKS);
        IndexInfo cninblocks("COUNT_NOTES_IN_BLOCK", COUNT_NOTES_IN_BLOCK);
        iout.write((char*)&cblocks, sizeof(IndexInfo));
        iout.write((char*)&cninblocks, sizeof(IndexInfo));
        for(int i=1; i<=COUNT_BLOCKS; i++){
            iout.seekp(COUNT_NOTES_IN_BLOCK * sizeof(KP));
            BorderInfo aboutblock("nulll");
            iout.write((char*)&aboutblock, sizeof(IndexInfo));
        }
        iout.close();
    }
    ofstream iout(INDEX_FILE_NAME, ios::binary);
    ofstream nout(NOTES_FILE_NAME, ios::binary);
}
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
            nin.close();
            cout << "OK" << endl;
            break;
        }
    }
    iin.close();
}


