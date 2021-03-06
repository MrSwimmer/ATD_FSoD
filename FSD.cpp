//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include <io.h>
#include <cstdlib>
#include <list>
#include <cstring>
#include <string.h>
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
        KP kp(const_cast<char *>("_"), 1);
        for (int i = 0; i < COUNT_NOTES_IN_BLOCK / 2; i++) {
            femp.write((char *) &kp, sizeof(KP));
        }
        femp.seekg(0);
        femp.read(emp, sizehalf);
        femp.close();
        fin.seekg(0);
        fin.write(emp, sizehalf);
        fin.write(emp, sizehalf);
        if(COUNT_NOTES_IN_BLOCK%2!=0) {
            fin.write((char *)&kp, sizeof(KP));
        }
        fin.seekg(0);
        KP kpn(const_cast<char *>("bnd"), 0);
        fin.write((char *) &kpn, sizeof(KP));
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
    if (local == -2) {
        return;
    }
    long long lastnote = begin + (COUNT_NOTES_IN_BLOCK - 1) * sizeof(KP);
    long long middle = begin + COUNT_NOTES_IN_BLOCK / 2 * sizeof(KP);
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    KP kp(ikey, static_cast<int>(point));
    in.write((char *) &kp, sizeof(KP));
    in.close();
}

void FSD::del(char *ikey) {
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlockToSearch(ikey, begin);//указатель на запись
    if (local == -1) {
        cout << "Not found" << endl;
        return;
    }
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    KP e(const_cast<char *>("_"), 0);
    in.write((char *) &e, sizeof(KP));
    in.close();
}

void FSD::find(char *ikey) {
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlockToSearch(ikey, begin);//указатель на запись
    if (local == -1) {
        cout << "Not found" << endl;
        return;
    }
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    KP kp;
    in.read((char *) &kp, sizeof(KP));
    in.close();
    fstream nn(NOTES_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(kp.pointer);
    int size;
    in.read((char *) &size, sizeof(int));
    int data;
    in.read((char *) &data, sizeof(int));
    in.close();
    cout << "LOCAL: block " << begin / COUNT_NOTES_IN_BLOCK / sizeof(KP) << " position " << kp.pointer << endl;
}

long long FSD::getLocalNoteInBlockToSearch(char *key, long long beginblock) {
    fstream fin(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    fin.seekg(beginblock);
    int size = COUNT_NOTES_IN_BLOCK* sizeof(KP);
    KP maskp[COUNT_NOTES_IN_BLOCK];
    fin.read(reinterpret_cast<char *>(maskp), size);
    for (int i = 0; i < COUNT_NOTES_IN_BLOCK; i++) {
        if (strcmp(maskp[i].key, key) == 0) {
            return beginblock + i* sizeof(KP);
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
    int c = 0;
    long p = 0;
    for (auto i : setblocks) {
        if(strcmp(key, i.key)==0) {
            return i.pointer;
        }
        if (strcmp(key, i.key)<0 || c == setblocks.size() - 1) {
            return p;
        }
        p = i.pointer;
        c++;
    }
}

void FSD::saveset() {
    fstream fin("set.txt", ios::binary | ios::out);
    cout << "SAVESET" << endl;
    int count = setblocks.size();
    fin.write((char *) &count, sizeof(int));
    for (auto i : setblocks) {
        cout << c << ". " << i.key << " " << i.pointer << endl;
        fin.write((char *) &i, sizeof(KP));
    }
    fin.close();
}

void FSD::getSetFromFile() {
    fstream in("set.txt", ios::binary | ios::in);
    setblocks.clear();
    in.seekg(0);
    cout << "GET_SET_FROM_FILE" << endl;
    int count;
    in.read((char *) &count, sizeof(int));
    for (int i = 0; i < count; i++) {
        KP kp;
        in.read((char *) &kp, sizeof(KP));
        setblocks.insert(kp);
        cout << i << ". " << kp.key << " " << kp.pointer << endl;
    }
}

long long FSD::getLocalNoteInBlock(char *key, long long beginblock) {
    fstream fin(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    fin.seekg(beginblock);
    KP kp, kpf;
    bool place = false;
    int pos;
    //fin.read((char *) &kpf, sizeof(KP));
    int size = COUNT_NOTES_IN_BLOCK* sizeof(KP);
    KP maskp[COUNT_NOTES_IN_BLOCK];
    fin.read(reinterpret_cast<char *>(maskp), size);
    for(int i=0; i<COUNT_NOTES_IN_BLOCK; i++) {
        pos=i;
        if(maskp[i].key=="_") {
            place=true;
            for(int j=i+1; j<COUNT_NOTES_IN_BLOCK; j++) {
                i=j;
                if(maskp[j].key!="_") {
                    if(comparemas(key, maskp[j].key)) {
                        return beginblock + (j-1)* sizeof(KP);
                    } else {
                        break;
                    }
                } else if (j == COUNT_NOTES_IN_BLOCK - 1) {
                    if (!comparemas(key, kpf.key)) {
                        return beginblock + pos * sizeof(KP);
                    }
                }
            }
        } else {
            kpf = maskp[i];
        }
    }
    /*for (int i = 1; i < COUNT_NOTES_IN_BLOCK; i++) {
        fin.read((char *) &kp, sizeof(KP));
        pos = i;
        if (kp.key == "_") {
            place = true;
            for (int j = i + 1; j < COUNT_NOTES_IN_BLOCK; j++) {
                i = j;
                fin.read((char *) &kp, sizeof(KP));
                if (kp.key != "_") {
                    if (comparemas(key, kp.key)) {
                        return fin.tellg() - sizeof(KP) * 2;
                    } else {
                        break;
                    }
                } else if (j == COUNT_NOTES_IN_BLOCK - 1) {
                    if (!comparemas(key, kpf.key)) {
                        return beginblock + pos * sizeof(KP);
                    }
                }
            }
        } else {
            kpf = kp;
        }
    }*/
    if(!place&&pos==COUNT_NOTES_IN_BLOCK-1) {
        getchar();
    }
    COUNT_BLOCKS++;
    fin.seekg(0, ios::end);
    long long begnewbl = fin.tellg();
    fin.write(emp, sizehalf);
    fin.write(emp, sizehalf);
    if(COUNT_NOTES_IN_BLOCK%2!=0) {
        KP kp(const_cast<char *>("_"), 1);
        fin.write((char *)&kp, sizeof(KP));
    }
    fin.close();
    KP kpnb(key, begnewbl);
    setblocks.insert(kpnb);
    return begnewbl;
}

KP::KP() {

}

bool KP::operator<(const KP &v) const {
    bool t = comparemas(this->key, v.key);
    return t;
}
