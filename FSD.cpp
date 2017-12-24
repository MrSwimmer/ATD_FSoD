//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include <io.h>
#include <cstdlib>
#include <list>

bool comparemas(char *a, char *b) {
    if(b=="_"){
        return true;
    } else {
        return a[0] < b[0];
    }

}
bool FileExists(const char *fname) {
    return access(fname, 0) != -1;
}
using namespace std;
FSD::FSD(int itemsInBlock) {
    if(FileExists(INDEX_FILE_NAME)){
        cout << "YUP" << endl;
    } else {
        cout << "NOPE";
        COUNT_NOTES_IN_BLOCK = itemsInBlock;
        ofstream iout(INDEX_FILE_NAME, ios::binary | ios::app);
        for(int i=0; i<itemsInBlock; i++){
            if(i==itemsInBlock-1){
                KP kp(const_cast<char *>("_"), -2);
                iout.write((char*)&kp, sizeof(KP));
            } else {
                KP kp(const_cast<char *>("_"), -1);
                iout.write((char*)&kp, sizeof(KP));
            }
        }
        iout.close();
    }
}
void FSD::getall() {
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    int count = 0;
    KP kpr;
    while (!iin.eof()) {
        iin.read((char*)&kpr, sizeof(KP));
        if(count==10) {
            break;
        }
        cout << count << " " << kpr.key << " loc " << kpr.pointer << endl;
        count++;
    }
    iin.close();
}
void FSD::insert(char* ikey, void* iNote) {//вставка записи
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long placeInsert = getLocalNoteIndex(ikey);//где
    fstream in(INDEX_FILE_NAME, ios::binary | ios::out | ios::in);
    in.seekg(placeInsert);
    KP kp(ikey, point);
    list<KP> listkp;
    listkp.push_back(kp);
    bool endblock = false;
    cout << placeInsert << endl;
    while (!endblock) {
        in.read((char*)&kp, sizeof(KP));
        //cout << "kp" << kp.key << endl;
        if(kp.key!="_") {
            listkp.push_back(kp);
        } else if(kp.pointer==-2) {
            //cout << "endblock" << endl;
            endblock = true;
        } else {
            endblock = true;
        }
    }
    //cout << "insert " << placeInsert << endl;
    //in.close();
    //ofstream iout(INDEX_FILE_NAME, ios::binary);
    in.seekp(placeInsert);
    for(int i=0; listkp.size()>0; i++) {
        in.write((char*)&listkp.front(), sizeof(KP));
        listkp.pop_front();
    }
    in.close();
    /*
    listkp.push_back()
    in.read((char*)&kp, sizeof(KP));

    ofstream iout(INDEX_FILE_NAME, ios::binary);

    iout.seekp(placeInsert);

    //cout << point << " " << placeInsert;
    /*ofstream iout(INDEX_FILE_NAME, ios::binary);
    KP newkp(ikey, point);//новый
    iout.seekp(placeInsert);
    iout.write((char*)&newkp, sizeof(KP));//пишем новый
    placeInsert+= sizeof(KP);
    iout.close();
    moveNotes(c, placeInsert);*/
}
/*void FSD::insert(char* ikey, void* iNote) {//вставка записи
    KP kpr;
    int c=1;
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long placeInsert = getLocalNoteIndex(ikey, kpr);//где
    ofstream iout(INDEX_FILE_NAME, ios::binary);
    KP newkp(ikey, point);//новый
    iout.seekp(placeInsert);
    iout.write((char*)&newkp, sizeof(KP));//пишем новый
    placeInsert+= sizeof(KP);
    iout.close();
    moveNotes(c, placeInsert);
}*/
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
                iin.close();
                break;
            }
        }
        div*=2;
        cout << "mid " << middle << endl;
    }
    iin.close();
    if(middle==0)
        middle=1;
    long long begin_block= ((middle-1)*COUNT_NOTES_IN_BLOCK)* sizeof(KP);
    return begin_block;
}
long long int FSD::getLocalNoteIndex(char* ikey) {
    long long local;
    long long begin = getBeginBlock(ikey);//получаем начало блока, в который надо положть запись
    //cout << "beginblock " << begin << endl;
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    iin.seekg(begin);
    for(int i=0; i<COUNT_NOTES_IN_BLOCK; i++) {
        KP kp;
        local = iin.tellg();
        iin.read((char*)&kp, sizeof(KP));//получаем запись из блока
        //cout << "compare keys: " << ikey << " " << kp.key << endl;
        if(comparemas(ikey, kp.key)) {
            //cout << "found" << endl;
            return local;
        } else {
            iin.seekg(begin+i* sizeof(KP));
        }
    }
    return begin;
}
/*long long int FSD::getLocalNoteIndex(char* ikey, KP kpr, int c = 0, int wh=0) {
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
}*/
void* FSD::getNote(char* ikey){
    KP kp;
    long long locget = getLocalNoteIndex(ikey);
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
    iout.seekp(getLocalNoteIndex(key));
    KP kp(const_cast<char *>("_"), -1);
    iout.write((char*)&kp, sizeof(KP));
    iout.close();
}
/*
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
                cout << count << " " << kpr.key << " loc " << kpr.pointer << endl;
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
}*/
