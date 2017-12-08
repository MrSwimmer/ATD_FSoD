//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
#include <iostream>
#include <io.h>

bool FileExists(const char *fname)
{
    return access(fname, 0) != -1;
}
using namespace std;
FSD::FSD() {
    ifstream nin(NOTES_FILE_NAME, ios::binary);

    if(FileExists(INDEX_FILE_NAME)){
        cout << "YUP";
    } else {
        cout << "NOPE";
        count_empty_blocks=COUNT_BLOCKS;
        ofstream iout(INDEX_FILE_NAME, ios::binary | ios::app);
        KP cblocks("COUNT_BLOCKS", COUNT_BLOCKS);
        KP cninblocks("COUNT_NOTES_IN_BLOCK", COUNT_NOTES_IN_BLOCK);
        iout.write((char*)&cblocks, sizeof(KP));
        iout.write((char*)&cninblocks, sizeof(KP));

        for(int i=2; i<=COUNT_BLOCKS*COUNT_NOTES_IN_BLOCK; i++){
            if(i==2||((i-2)%COUNT_NOTES_IN_BLOCK==0&&i>1000)){
                if(COUNT_SYMBOLS_IN > 0) {
                    char first_in_block = COUNT_SYMBOLS_IN * ((i - 2) / COUNT_NOTES_IN_BLOCK);
                    char *hh = new char{first_in_block};
                    long long point = addToEnd(hh, sizeof(hh));
                    cout << endl << "OK p " << point  << " symbol " << first_in_block << endl;
                    KP kp(hh, point);
                    iout.write((char*)&kp, sizeof(KP));
                    continue;
                }
            }
            KP kp("null", 0);
            iout.write((char*)&kp, sizeof(KP));
        }
        iout.close();
    }
}

long long FSD::addToEnd(void* iNote, int size) {
    ofstream nout("notes.txt", ios::binary | ios::app);

    fstream file("notes.txt");

    long long point;
    file.seekg (0, ios::end);
    point = file.tellg();

    nout.write((char*)&size, sizeof(int));
    nout.write((char*)iNote, size);
    nout.close();

    return point;
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
    int middle = COUNT_BLOCKS/2;
    KP kpr;
    while (middle!=0){
        iin.seekg((middle*COUNT_NOTES_IN_BLOCK+2)* sizeof(KP));
        iin.read((char*)&kpr, sizeof(KP));
        if(key < kpr.key) {
            middle/=2;
        } else {
            middle+=middle/2;
        }
        cout << "mid " << middle << endl;
    }
    iin.seekg((middle*COUNT_NOTES_IN_BLOCK+2)* sizeof(KP));
    for (int i = 0; i < COUNT_NOTES_IN_BLOCK; i++) {
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
void FSD::getall() {
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    ifstream nin(NOTES_FILE_NAME, ios::binary);
    KP info;
    iin.read((char*)&info, sizeof(KP));
    iin.read((char*)&info, sizeof(KP));
    KP kpr;
    while (!iin.eof()){
        iin.read((char*)&kpr, sizeof(KP));
        cout << kpr.key;
    }
    iin.close();
}


