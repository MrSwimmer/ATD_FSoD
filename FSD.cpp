//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include <io.h>
#include <cstdlib>
#include <list>

bool comparemas(char *a, char *b) {
    return a[0] < b[0];
}
bool FileExists(const char *fname) {
    return access(fname, 0) != -1;
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
                    long long point = addToEnd(&hh[0]);
                    KP kp(hh, point);
                    iout.write((char*)&kp, sizeof(KP));
                    continue;
                }
            } else {
                KP kp(const_cast<char *>("_"), -1);
                iout.write((char*)&kp, sizeof(KP));
            }
        }
        iout.close();
    }
}
long long FSD::addToEnd(void* iNote) { //помещение записи в конец файла с записями
    ofstream nout("notes.txt", ios::binary | ios::app);
    fstream file("notes.txt");

    int size = sizeof(iNote);
    long long point;
    file.seekg (0, ios::end);
    point = file.tellg();

    nout.write((char*)&size, sizeof(int));
    nout.write((char*)iNote, size);
    nout.close();

    return point;
}
void FSD::insert(char* ikey, void* iNote) {//вставка записи
    KP kpr;
    int c=1;
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long placeInsert = getLocalNoteIndex(ikey, kpr, c, 0);//где
    ofstream iout(INDEX_FILE_NAME, ios::binary);
    KP newkp(ikey, point);//новый
    iout.seekp(placeInsert);
    iout.write((char*)&newkp, sizeof(KP));//пишем новый
    placeInsert+= sizeof(KP);
    iout.close();
    moveNotes(c, placeInsert);
}
void FSD::moveNotes(int c, long long placeInsert){
    KP kpr, buf;
    ofstream iout(INDEX_FILE_NAME, ios::binary);
    iout.seekp(placeInsert);
    for(int j=c; j<COUNT_NOTES_IN_BLOCK-1; j++) {//смещаем все записи
        ifstream iin(INDEX_FILE_NAME, ios::binary);
        iin.read((char*)&kpr, sizeof(KP));
        iin.close();
        iout.write((char*)&buf, sizeof(KP));
        buf=kpr;
        placeInsert+= sizeof(KP);
    }
    iout.close();
}
long long FSD::getBeginBlock(char* key){
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    KP kpr;
    int middle = COUNT_BLOCKS/2;
    int div=2;
    int topdiv=COUNT_BLOCKS-1;
    while (COUNT_BLOCKS-middle!=1&&middle!=topdiv){
        iin.seekg((middle*COUNT_NOTES_IN_BLOCK)* sizeof(KP));
        iin.read((char*)&kpr, sizeof(KP));
        if(comparemas(key, kpr.key)) {
            middle/=2;
        } else {
            int nd = middle/div;
            if(nd!=0){
                middle+=nd;
            } else {
                middle++;
                break;
            }
        }
        div*=2;
        cout << "mid " << middle << endl;
    }
    long long begin_block= ((middle-1)*COUNT_NOTES_IN_BLOCK)* sizeof(KP);
    return begin_block;
}
long long int FSD::getLocalNoteIndex(char* ikey, KP kpr, int c = 0, int wh=0) {
    long long begin = getBeginBlock(ikey);//получаем начало блока, в который надо положть запись
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    KP buf;
    if(wh==0){
        iin.seekg(begin);//переходим к началу блока
    } else {
        iin.seekg(begin+wh* sizeof(KP));
    }
    for (int i = wh; i < COUNT_NOTES_IN_BLOCK; i++) {
        c=i;
        iin.read((char*)&kpr, sizeof(KP));//получаем запись из блока
        long long local = iin.tellg();
        if(comparemas(ikey, kpr.key)){
            iin.close();
            long long local = begin+(i* sizeof(KP));//где
            return local;
        }
        if(kpr.key=="_") {
            for(int j=i+1; j<COUNT_NOTES_IN_BLOCK; j++){
                iin.read((char*)&kpr, sizeof(KP));
                if(kpr.key!="_"){
                    iin.close();
                    long long local = begin+(i* sizeof(KP));//где
                    getLocalNoteIndex(ikey, kpr, c, j);
                }
            }
        }
    }
}
void* FSD::getNote(char* ikey){
    KP kp;
    long long locget = getLocalNoteIndex(ikey, kp);
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    iin.seekg(locget);
    KP buf;
    iin.read((char*)&buf, sizeof(KP));
    iin.close();
    long long point = buf.pointer;
    return getNoteSup(point);
}
void* FSD::getNoteSup(long long point){
    void* note;
    ifstream nin(NOTES_FILE_NAME, ios::binary);
    nin.seekg(point);
    int sizenote;
    nin.read((char*)sizenote, sizeof(int));
    nin.read((char*)note, sizenote);
    nin.close();
    return note;
}
void FSD::delNote(char* key){
    ofstream iout(INDEX_FILE_NAME, ios::binary);
    KP kpr;
    iout.seekp(getLocalNoteIndex(key, kpr));
    KP kp(const_cast<char *>("_"), -1);
    iout.write((char*)&kp, sizeof(KP));
    iout.close();
}
void FSD::getall(list<char*> notes) {
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    int count = 0;
    KP kpr;
    while (!iin.eof()){
        iin.read((char*)&kpr, sizeof(KP));
        if(count!=0) {
            try {
                if(kpr.key==NULL) {
                    throw 0;
                }
                cout << count << " " << kpr.key << "loc " << iin.tellg() << endl;
                //notes.push_back(getNoteSup(kpr.pointer));
            } catch (int a) {
                if(a==0) {
                    cout << "null" << endl;
                }
            }
        }
        count++;
    }
    iin.close();
}