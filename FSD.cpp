//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
#include <iostream>
#include <io.h>
#include <cstdlib>

bool FileExists(const char *fname)
{
    return access(fname, 0) != -1;
}
char * randomString (int sybols = 3) {
    char *str;
    str = new char [sybols+1];
    for (int i = 0; i < sybols; i++)
    {
        str [i]  = static_cast<char>(rand());
    }
    str[sybols] = '\0';
    return str;
    cout << str;
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
            cout << kp.key;
            iout.write((char*)&kp, sizeof(KP));
        }
        iout.close();
    }
}
void FSD::fillInAll() {
    ofstream iout(INDEX_FILE_NAME, ios::binary | ios::app);
    for(int i=2; i<=COUNT_BLOCKS*COUNT_NOTES_IN_BLOCK; i++){
        if(i==2||((i-2)%COUNT_NOTES_IN_BLOCK==0&&i>1000)){

        } else {
            KP kp(randomString(), -1);
            char *q = "11111";
            insert(randomString(), q, sizeof(KP));
        }
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

    ofstream iout(INDEX_FILE_NAME, ios::binary);
    ifstream iin(INDEX_FILE_NAME, ios::binary);

    long long point = addToEnd(iNote, size);//добавляем в конец notes запись, в point указатель
    long long begin = getBeginBlock(ikey);//получаем начало блока, в который надо положть запись
    KP kpr, buf;
    iin.seekg(begin);//переходим к началу блока
    for (int i = 0; i < COUNT_NOTES_IN_BLOCK; i++) {
        //получаем запись из блока
        iin.read((char*)&kpr, sizeof(KP));
        cout << ikey << " " << kpr.key;
        if(kpr.key>ikey){
            long long placeInsert = begin+(i* sizeof(KP));//где
            KP newkp(ikey, point);//новый
            iout.seekp(placeInsert);
            iout.write((char*)&newkp, sizeof(KP));//пишем новый
            buf = kpr;//запоминаем старый
            placeInsert+= sizeof(KP);
            for(int j=i; j<COUNT_NOTES_IN_BLOCK||buf.key!="null"; j++) {
                iin.seekg(placeInsert);
                iin.read((char*)&kpr, sizeof(KP));
                iout.seekp(placeInsert);
                iout.write((char*)&buf, sizeof(KP));
                buf=kpr;
                placeInsert+= sizeof(KP);
            }
            moveNotesInBlock(placeInsert, i);
            break;
        }
    }
    iin.close();
    iout.close();
}
void FSD::moveNotesInBlock(long long place, int i) {
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    ofstream iout(INDEX_FILE_NAME, ios::binary);

    iin.seekg(place);
    KP kpr;
    for(; i<COUNT_NOTES_IN_BLOCK; i++) {
        iin.read((char*)&kpr, sizeof(KP));
        long long placeInsert = place+(i* sizeof(KP));
        iout.seekp(placeInsert);

    }
}
long long FSD::getBeginBlock(char* key){
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    KP kpr;
    int middle = COUNT_BLOCKS/2;
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
    long long begin_block= (middle*COUNT_NOTES_IN_BLOCK+2)* sizeof(KP);
    return begin_block;
}
void FSD::getnote(char *dest, char* key) {
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    ifstream nin(NOTES_FILE_NAME, ios::binary);
    long long middle = getBeginBlock(key);
    KP kpr;
    iin.seekg(middle);
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


