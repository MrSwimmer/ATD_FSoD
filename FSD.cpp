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
                KP kp(const_cast<char *>("_"), -8000);
                iout.write((char*)&kp, sizeof(KP));
            } else {
                if(i==0) {
                    KP kp(const_cast<char *>("bnd"), 0);
                    iout.write((char*)&kp, sizeof(KP));
                    long long n = 0;
                    KP block(const_cast<char *>("bnd"), n);
                    blocks[0]=block;
                } else {
                    KP kp(const_cast<char *>("_"), 1);
                    iout.write((char*)&kp, sizeof(KP));
                }
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
        cout << count << " " << kpr.key << " loc " << kpr.pointer << endl;
        count++;
    }
    iin.close();
}
void FSD::insert(char* ikey, void* iNote) {//вставка записи
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long mid;
    long long placeInsert = getLocalNoteIndex(ikey, mid);//где
    fstream in(INDEX_FILE_NAME, ios::binary | ios::out | ios::in);
    in.seekg(placeInsert);
    KP kp(ikey, point);
    list<KP> listkp;
    listkp.push_back(kp);
    bool endblock = false;
    while (!endblock) {
        in.read((char*)&kp, sizeof(KP));
        if(kp.key!="_") {
            listkp.push_back(kp);
        } else if(kp.pointer<0) {
            cout << "endblock";
            mid =(COUNT_NOTES_IN_BLOCK/2)* sizeof(KP);
            in.seekg(mid);
            list<KP> mlistkp;
            char *beg;
            for(int i=COUNT_NOTES_IN_BLOCK/2; i<COUNT_NOTES_IN_BLOCK-1; i++) {
                in.read((char*)&kp, sizeof(KP));
                mlistkp.push_back(kp);
                if(i==COUNT_NOTES_IN_BLOCK/2){
                    beg = kp.key;
                }
            }
            KP block(beg, COUNT_NOTES_IN_BLOCK*COUNT_BLOCKS*sizeof(KP));
            for(int i=0; i<COUNT_BLOCKS; i++) {
                if(comparemas(block.key,blocks[i].key)) {
                    KP buf = blocks[i];
                    blocks[i]=block;
                    blocks[i+1]=buf;
                    for(int j=i+2; j<COUNT_BLOCKS-1; j++) {
                        blocks[j]=blocks[j+1];
                    }
                }
            }
            //blocks.insert(block);
            in.seekg(COUNT_NOTES_IN_BLOCK*COUNT_BLOCKS* sizeof(KP));
            COUNT_BLOCKS++;
            for(int i=0; i<COUNT_NOTES_IN_BLOCK/2; i++) {
                in.write((char*)&mlistkp.front(), sizeof(KP));
                mlistkp.pop_front();
            }
            for(int i=COUNT_NOTES_IN_BLOCK/2; i<COUNT_NOTES_IN_BLOCK; i++) {
                if(i==COUNT_NOTES_IN_BLOCK-1) {
                    KP kpe(const_cast<char *>("_"), -1);
                    in.write((char*)&kpe, sizeof(KP));
                } else {
                    KP kpe(const_cast<char *>("_"), 1);
                    in.write((char*)&kpe, sizeof(KP));
                }
            }
            in.seekg(mid);
            for(int i=COUNT_NOTES_IN_BLOCK/2; i<COUNT_NOTES_IN_BLOCK-1; i++) {
                KP kpe(const_cast<char *>("_"), 1);
                in.write((char*)&kpe, sizeof(KP));
            }
            in.close();
            return;
            endblock = true;
        } else {
            endblock = true;
        }
    }
    in.seekp(placeInsert);
    for(int i=0; listkp.size()>0; i++) {
        in.write((char*)&listkp.front(), sizeof(KP));
        listkp.pop_front();
    }
    in.close();
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

long long FSD::getBeginBlock(char* key){

    /*int midd;
    int left = 0;
    int right = COUNT_BLOCKS;
    while(1){
        midd = (left + right) / 2;
        if (key < blocks[midd].key)       // если искомое меньше значения в ячейке
            right = midd - 1;      // смещаем правую границу поиска
        else if (key > blocks[midd].key)  // если искомое больше значения в ячейке
            left = midd + 1;	   // смещаем левую границу поиска
        else                       // иначе (значения равны)
            return midd;           // функция возвращает индекс ячейки
        if (left > right)          // если границы сомкнулись
            return midd;
    }*/
    return 0;
}
/*long long FSD::getBeginBlock(char* key){
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
}*/
long long int FSD::getLocalNoteIndex(char* ikey, long long mid) {
    long long local;
    long long begin = getBeginBlock(ikey);//получаем начало блока, в который надо положть запись
    mid = begin;
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    iin.seekg(begin);
    for(int i=0; i<COUNT_NOTES_IN_BLOCK; i++) {
        KP kp;
        local = iin.tellg();
        iin.read((char*)&kp, sizeof(KP));//получаем запись из блока
        if(comparemas(ikey, kp.key)) {

            return local;
        } else {
            iin.seekg(begin+i* sizeof(KP));
        }
    }
    return begin;
}/*
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
}*/