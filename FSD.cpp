//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
#include <iostream>
#include <io.h>
#include <cstdlib>
#include <ctime>

bool FileExists(const char *fname)
{
    return access(fname, 0) != -1;
}
char * randomString (int sybols = 3) {
    char *str;
    str = new char [sybols+1];
    for (int i = 0; i < sybols; i++)
    {
        str [i]  = static_cast<char>(97 + rand() % 10);
    }
    str[sybols] = '\0';
    return str;
}
using namespace std;
FSD::FSD() {
    ifstream nin(NOTES_FILE_NAME, ios::binary);

    if(FileExists(INDEX_FILE_NAME)){
        cout << "YUP";
    } else {
        cout << "NOPE";
        ofstream iout(INDEX_FILE_NAME, ios::binary | ios::app);
        for(int i=0; i<COUNT_BLOCKS*COUNT_NOTES_IN_BLOCK; i++){
            if(i==0||(i%COUNT_NOTES_IN_BLOCK==0&&i>=COUNT_NOTES_IN_BLOCK)){
                if(COUNT_SYMBOLS_IN > 0) {
                    char first_in_block = COUNT_SYMBOLS_IN * (i / COUNT_NOTES_IN_BLOCK);
                    string str = first_in_block+".";
                    char *hh = new char[1];
                    hh[0]=first_in_block;
                    cout << "fsib " << hh << endl;
                    long long point = addToEnd(&hh[0], sizeof(hh));
                    KP kp(hh, point);
                    iout.write((char*)&kp, sizeof(KP));
                    continue;
                }
            } else {
                KP kp("_", -1);
                iout.write((char*)&kp, sizeof(KP));

            }
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

    ofstream iout(INDEX_FILE_NAME, ios::binary);
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    long long point = addToEnd(iNote, size);//добавляем в конец notes запись, в point указатель
    long long begin = getBeginBlock(ikey);//получаем начало блока, в который надо положть запись
    cout << begin/ sizeof(KP) << endl;
    KP kpr, buf;
    iin.seekg(begin);//переходим к началу блока
    for (int i = 0; i < COUNT_NOTES_IN_BLOCK; i++) {
        //получаем запись из блока
        //ERROR! при чтении объекта происходит ошибка - tellg = -1 (место работы в файле)
        iin.read((char*)&kpr, sizeof(KP));
        long long local = iin.tellg();
        if(kpr.key>ikey){
            long long placeInsert = begin+(i* sizeof(KP));//где
            KP newkp(ikey, point);//новый
            iout.seekp(placeInsert);
            iout.write((char*)&newkp, sizeof(KP));//пишем новый
            buf = kpr;//запоминаем старый
            placeInsert+= sizeof(KP);
            /*for(int j=i; j<COUNT_NOTES_IN_BLOCK&&buf.key!="_"; j++) {
                iin.read((char*)&kpr, sizeof(KP));
                iout.write((char*)&buf, sizeof(KP));
                buf=kpr;
                placeInsert+= sizeof(KP);
            }*/
            break;
        }
    }
    iin.close();
    iout.close();
}
long long FSD::getBeginBlock(char* key){
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    KP kpr;
    int middle = COUNT_BLOCKS/2;
    int div=2;
    int topdiv=COUNT_BLOCKS-1;
    while (middle!=0&&middle!=topdiv){
        iin.seekg((middle*COUNT_NOTES_IN_BLOCK)* sizeof(KP));
        iin.read((char*)&kpr, sizeof(KP));
        string str = key;
        string sec = kpr.key;
        if(key < kpr.key) {
            middle/=2;
        } else {
            middle+=middle/div;
        }
        div*=2;
        cout << "mid " << middle << endl;
    }
    long long begin_block= ((middle-1)*COUNT_NOTES_IN_BLOCK)* sizeof(KP);
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
    int count = 0;
    KP kpr;
    while (!iin.eof()){
        iin.read((char*)&kpr, sizeof(KP));
        if(count!=0) {
            string str = kpr.key;
            cout << count << " " << str << "loc " << iin.tellg() << endl;
        }
        count++;
    }
    iin.close();
}


