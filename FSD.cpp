//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include <io.h>
#include <cstdlib>
bool comparemas(char *a, char *b) {
    return a[0] < b[0];
}
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
                KP kp("_", -1);
                iout.write((char*)&kp, sizeof(KP));
            }
        }
        iout.close();
    }
}
long long FSD::addToEnd(void* iNote) {
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
void FSD::insert(char* ikey, void* iNote) {

    KP kpr, buf;
    int c=1;
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long placeInsert = getLocalNote(ikey, kpr, c);//где
    ofstream iout(INDEX_FILE_NAME, ios::binary);
    KP newkp(ikey, point);//новый
    iout.seekp(placeInsert);
    iout.write((char*)&newkp, sizeof(KP));//пишем новый
    buf = kpr;//запоминаем старый
    placeInsert+= sizeof(KP);
    for(int j=c; j<COUNT_NOTES_IN_BLOCK&&buf.key!="_"; j++) {
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
long long int FSD::getLocalNote(char* ikey, KP kpr = kpr, int c = 0) {
    long long begin = getBeginBlock(ikey);//получаем начало блока, в который надо положть запись
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    KP buf;
    iin.seekg(begin);//переходим к началу блока
    for (int i = 0; i < COUNT_NOTES_IN_BLOCK; i++) {
        c=i;
        iin.read((char*)&kpr, sizeof(KP));//получаем запись из блока
        long long local = iin.tellg();
        if(comparemas(ikey, kpr.key)||kpr.key=="_"){
            iin.close();
            long long local = begin+(i* sizeof(KP));//где
            return local;
        }
    }
}
void FSD::getNote(char* ikey, void* buf){
    long long locget = getLocalNote(ikey);
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    iin.seekg(locget);
    iin.read((char*)buf, sizeof(buf));
}
void FSD::getall() {
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