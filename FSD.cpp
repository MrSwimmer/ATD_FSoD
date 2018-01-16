//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include <io.h>
#include <cstdlib>
#include <list>
#include <cstring>

bool FileExists(const char *fname) {
    return access(fname, 0) != -1;
}

bool comparemas(char *a, char *b) {
    if (b == "_") {
        return true;
    } else {
        for (int i = 0; a[0] != '\0'; i++) {
            if (a[i] < b[i]) {
                return true;
            } else if (a[i] > b[i]) {
                return false;
            }
        }
        return true;
    }
}
using namespace std;

FSD::FSD(int itemsInBlock) {
    setblocks.clear();
    COUNT_NOTES_IN_BLOCK = itemsInBlock;
    sizehalf = (COUNT_NOTES_IN_BLOCK / 2) * sizeof(KP);
    emp = new char[sizehalf];
    if (FileExists(INDEX_FILE_NAME)) {
        cout << "YUP" << endl;
        getSetFromFile();
    } else {
        fstream fin(INDEX_FILE_NAME, ios::binary | ios::out);
        fstream femp(EMP_FILE_NAME, ios::binary | ios::app | ios::out | ios::in);
        for (int i = 0; i < COUNT_NOTES_IN_BLOCK/2; i++) {
            KP kp(const_cast<char *>("_"), 1);
            femp.write((char *)&kp, sizeof(KP));
        }
        femp.seekg(0);
        femp.read(emp, sizehalf);
        femp.close();
        fin.seekg(0);
        fin.write(emp, sizehalf);
        fin.write(emp, sizehalf);
        fin.seekg(0);
        KP kp(const_cast<char *>("bnd"), 0);
        fin.write((char*)&kp, sizeof(KP));
        KP block(const_cast<char *>("bnd"), 0);
        setblocks.insert(block);
        fin.close();
    }
}

void FSD::getall() {
    ifstream iin(INDEX_FILE_NAME, ios::binary);
    int count = 0;
    KP kpr;
    while (!iin.eof()) {
        iin.read((char *) &kpr, sizeof(KP));
        cout << count << " " << kpr.key << " loc " << kpr.pointer << endl;
        count++;
    }
    iin.close();
}

void FSD::insert(char *ikey, void *iNote) {//вставка записи
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlock(ikey, begin);//указатель на запись
    long long lastnote = begin + (COUNT_NOTES_IN_BLOCK-1)* sizeof(KP);
    long long middle = begin + COUNT_NOTES_IN_BLOCK/2* sizeof(KP);
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    KP kp(ikey, static_cast<int>(point));
    in.write((char *) &kp, sizeof(KP));
    in.close();
}
void FSD::del(char *ikey) {
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlockToSearch(ikey, begin);//указатель на запись
    if(local==-1) {
        cout << "Not found" << endl;
        return;
    }
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local* sizeof(KP) + begin);
    KP e("_", 0);
    in.write((char*)&e, sizeof(KP));
    in.close();
}
void FSD::find(char *ikey) {
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlockToSearch(ikey, begin);//указатель на запись
    if(local==-1) {
        cout << "Not found" << endl;
        return;
    }
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    KP kp;
    in.read((char*)&kp, sizeof(KP));
    in.close();
    cout << "LOCAL: block " << begin/COUNT_NOTES_IN_BLOCK/ sizeof(KP) << " position " << local << endl;
}
long long FSD::getLocalNoteInBlockToSearch(char *key, long long beginblock) {
    fstream fin(INDEX_FILE_NAME, ios::binary | ios::in );
    fin.seekg(beginblock);
    KP kp;
    for(int i=0; i<COUNT_NOTES_IN_BLOCK; i++) {
        fin.read((char *) &kp, sizeof(KP));
        if(strcmp(kp.key,key)==0) {
            int now = fin.tellg();
            fin.seekg(now- sizeof(KP));
            long local = fin.tellg();
            return local;
        }
    }
    return -1;
}
long long FSD::addToEnd(void *iNote) { //помещение записи в конец файла с записями
    ofstream nout("notes.txt", ios::binary | ios::app);
    fstream file("notes.txt");
    int size = sizeof(iNote);
    long long point;
    file.seekg(0, ios::end);
    point = file.tellg();
    nout.write((char *) &size, sizeof(int));
    nout.write((char *) iNote, size);
    nout.close();
    return point;
}
long long FSD::getBeginBlock(char *key) {
    int c=0;
    long p=0;
    for (auto i : setblocks) {
        if (comparemas(key, i.key)|| c==setblocks.size()-1){
            return p;
        }
        p=i.pointer;
        c++;
    }
}
void FSD::saveset() {
    fstream fin("set.txt", ios::binary | ios::out);
    cout << "SAVESET" << endl;
    int count = setblocks.size();
    fin.write((char*)&count, sizeof(int));
    for (auto i : setblocks) {
            cout << c << ". " << i.key << " " << i.pointer << endl;
            fin.write((char*)&i, sizeof(KP));
    }
    fin.close();
}
void FSD::getSetFromFile() {
    fstream in("set.txt", ios::binary | ios::in);
    setblocks.clear();
    in.seekg(0);
    cout << "GET_SET_FROM_FILE" << endl;
    int count;
    in.read((char*)&count, sizeof(int));
    for(int i=0; i<count; i++) {
        KP kp;
        in.read((char*)&kp, sizeof(KP));
        setblocks.insert(kp);
        cout << i << ". " << kp.key << " " << kp.pointer << endl;
    }
}
long long FSD::getLocalNoteInBlock(char *key, long long beginblock) {
    fstream fin(INDEX_FILE_NAME, ios::binary | ios::in | ios::out );
    fin.seekg(beginblock);
    KP kp, kpf;
    int pos;
    fin.read((char *) &kpf, sizeof(KP));
    bool found = false;
    for(int i=1; i<COUNT_NOTES_IN_BLOCK; i++) {
        fin.read((char *) &kp, sizeof(KP));
        pos = i;
        if (kp.key == "_") {
            for(int j=i+1; j<COUNT_NOTES_IN_BLOCK; j++) {
                i=j;
                fin.read((char *) &kp, sizeof(KP));
                if(kp.key!="_") {
                    if(comparemas(key, kp.key)) {
                        return fin.tellg()- sizeof(KP)*2;
                    } else {
                        break;
                    }
                } else if(j==COUNT_NOTES_IN_BLOCK-1) {
                    if(!comparemas(key, kpf.key)) {
                        return beginblock+pos*sizeof(KP);
                    }
                }
            }
        } else {
            kpf=kp;
        }
    }
    if(!found) {
        COUNT_BLOCKS++;
        fin.seekg(0, ios::end);
        long long begnewbl = fin.tellg();
        fin.write(emp, sizehalf);
        fin.write(emp, sizehalf);
        fin.close();
        KP kpnb(key, begnewbl);
        setblocks.insert(kpnb);
        return begnewbl;

    }
}
KP::KP() {

}
bool KP::operator<(const KP &v) const {
    bool t = comparemas(this->key, v.key);
    return t;
}
