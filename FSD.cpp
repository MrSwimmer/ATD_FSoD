//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include <io.h>
#include <cstdlib>
#include <list>

bool FileExists(const char *fname) {
    return access(fname, 0) != -1;
}

bool comparemas(char *a, char *b) {
    if(b=="_") {
        return true;
    } else {
        for(int i=0; a[0]!='\0'; i++) {
            if(a[i]<b[i]) {
                return true;
            } else if (a[i]>b[i]) {
                return false;
            }
        }
        return true;
    }
}
using namespace std;
FSD::FSD(int itemsInBlock) {
    if(FileExists(INDEX_FILE_NAME)){
        cout << "YUP" << endl;
    } else {
        cout << "NOPE";
        setblocks.clear();
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
                    KP block(const_cast<char *>("bnd"), 0);
                    setblocks.insert(block);
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
    getchar();
}
void FSD::insert(char* ikey, void* iNote) {//вставка записи
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long begin = getBeginBlock(ikey);
    long long local = getLocalNoteInBlock(ikey, begin);
    cout << begin << " " << local;
    fstream in(INDEX_FILE_NAME, ios::binary | ios::out | ios::in);
    /*in.seekg(placeInsert);
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
            getall();
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
            setblocks.insert(block);
            //blocks[COUNT_BLOCKS]=block;
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
    }*/
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
    int div=2;
    long long beg=0;
    int size=setblocks.size();
    int loc=size/div;
    while(true) {
        int c=0;
        char* keyset;
        for(auto i : setblocks) {
            if(c==loc) {
                keyset = i.key;
                break;
            }
            c++;
        }
        if(comparemas(key, keyset)) {
            if(div*2>size) {
                beg = loc-size/div/2-1;
                break;
            } else {
                div*=2;
                loc-=size/div;
            }
        } else {
            if(div>size) {
                beg =  loc+size/div/2-1;
                break;
            } else {
                div*=2;
                loc+=size/div;
            }
        }
    }
    if(beg<0) {
        beg=0;
    }
    return beg;
}
long long FSD::getLocalNoteInBlock(char* key, long long beginblock){
    int div=2;
    long long beg=0;
    int size=COUNT_NOTES_IN_BLOCK;
    int loc=size/div;
    fstream fin("index.txt", ios::binary | ios::in);
    while(true) {
        int c=0;
        char* keyset;
        int f=(loc+beginblock*COUNT_NOTES_IN_BLOCK)* sizeof(KP);
        fin.seekg(f);
        KP kp;
        fin.read((char*)&kp, sizeof(KP));
        keyset = kp.key;
        if(comparemas(key, keyset)) {
            if(div*2>size) {
                beg = loc-size/div/2-1;
                break;
            } else {
                div*=2;
                loc-=size/div;
            }
        } else {
            if(div>size) {
                beg =  loc+size/div/2-1;
                break;
            } else {
                div*=2;
                loc+=size/div;
            }
        }
    }
    return (beg+COUNT_NOTES_IN_BLOCK*beginblock)*sizeof(KP);
}
/*long long FSD::getLocalNoteIndex(char* ikey, long long mid) {
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
        }
    }
    return begin;
}
void* FSD::getNote(char* ikey){
    KP kp;
    long long locget = getLocalNoteIndex(ikey, 0);
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
    fstream iout(INDEX_FILE_NAME, ios::binary | ios::out | ios::in);
    KP kp;
    list<KP> mlistkp;
    long long loc = getLocalNoteIndex(key, 0);
    iout.seekp(loc);
    for(int i=(loc%COUNT_NOTES_IN_BLOCK)/ sizeof(KP); i<COUNT_NOTES_IN_BLOCK-1; i++) {
        iout.read((char*)&kp, sizeof(KP));
        mlistkp.push_back(kp);
    }
    iout.seekg(loc);
    for(int i=(loc%COUNT_NOTES_IN_BLOCK)/ sizeof(KP); i<COUNT_NOTES_IN_BLOCK-1; i++) {
        iout.write((char*)&mlistkp.front(), sizeof(KP));
        mlistkp.pop_front();
    }
    iout.close();
}*/
KP::KP() {

}

bool KP::operator<(const KP &v) const {
    bool t = comparemas(this->key, v.key);
    return t;
}
