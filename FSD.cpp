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
        fstream fin(INDEX_FILE_NAME, ios::binary | ios::out);
        for(int i=0; i<COUNT_BLOCKS; i++) {
            KP note;
            fin.seekg(i*COUNT_BLOCKS* sizeof(KP));
            fin.read((char*)&note, sizeof(KP));
            KP block(note.key, fin.tellg());
            setblocks.insert(block);
        }
        fin.close();
    } else {
        fstream fin(INDEX_FILE_NAME, ios::binary | ios::out);
        cout << "NOPE";
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
    getchar();
    iin.close();
}

void FSD::insert(char *ikey, void *iNote) {//вставка записи
    long long point = addToEnd(iNote);//добавляем в конец notes запись, в point указатель
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlock(ikey, begin);//указатель на запись
    local = local* sizeof(KP) + begin;
    long long lastnote = begin + (COUNT_NOTES_IN_BLOCK-1)* sizeof(KP);
    long long middle = begin + COUNT_NOTES_IN_BLOCK/2* sizeof(KP);
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    int sizerest = begin + (COUNT_NOTES_IN_BLOCK-1)* sizeof(KP) - local;
    char *rest = new char[sizerest];
    in.read(rest, sizerest);
    in.seekp(local);
    KP kp(ikey, static_cast<int>(point));
    in.write((char *) &kp, sizeof(KP));
    in.write(rest, sizerest);
    KP last;
    in.seekg(lastnote);
    in.read((char *) &last, sizeof(KP));
    if (last.key != "_") {
        COUNT_BLOCKS++;
        char *half = new char[sizehalf];
        in.seekg(middle);
        KP beginbl;
        in.read((char*)&beginbl, sizeof(KP));
        in.seekg(middle);
        in.read(half, sizehalf);
        in.seekg(middle);
        in.write(emp, sizehalf);
        in.seekg(0, ios::end);
        long long begnewbl = in.tellg();
        in.write(half, sizehalf);
        in.write(emp, sizehalf);
        delete[]half;
        KP kpnb(beginbl.key, begnewbl);
        setblocks.insert(kpnb);
    }
    in.close();
}
void FSD::del(char *ikey) {
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlock(ikey, begin);//указатель на запись
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    int sizerest = begin + (COUNT_NOTES_IN_BLOCK-1)* sizeof(KP) - local;
    char *rest = new char[sizerest];
    in.read(rest, sizerest);
    in.seekp(local - sizeof(KP));
    in.write(rest, sizerest);
    KP e("_", 0);
    in.write((char*)&e, sizeof(KP));
}
void* FSD::find(char *ikey) {
    long long begin = getBeginBlock(ikey);//начало блока
    long long local = getLocalNoteInBlock(ikey, begin);//указатель на запись
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);
    in.seekg(local);
    KP kp;
    in.read((char*)&kp, sizeof(KP));
    in.close();
    fstream nn(NOTES_FILE_NAME, ios::binary | ios::in | ios::out);
    nn.seekg(kp.pointer);
    int size;
    void* data;
    nn.read((char*)&size, sizeof(int));
    nn.read((char*)&data, size);
    nn.close();
    cout << data;
    return data;
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
    for (auto i : setblocks) {
        if (comparemas(key, i.key)|| c==setblocks.size()-1){
            return i.pointer;
        }
        c++;
    }
}

long long FSD::getLocalNoteInBlock(char *key, long long beginblock) {
    int div = 2;
    long long beg = 0;
    int size = COUNT_NOTES_IN_BLOCK;
    int loc = size / div;
    fstream fin("index.txt", ios::binary | ios::in);
    while (true) {
        int c = 0;
        char *keyset;
        int f = loc * sizeof(KP) + beginblock;
        fin.seekg(f);
        KP kp;
        fin.read((char *) &kp, sizeof(KP));
        keyset = kp.key;
        if (comparemas(key, keyset)) {
            if (div * 2 > size) {
                beg = loc - size / div / 2 - 1;
                break;
            } else {
                div *= 2;
                loc -= size / div;
            }
        } else {
            if (div > size) {
                beg = loc + size / div / 2 - 1;
                break;
            } else {
                div *= 2;
                loc += size / div;
            }
        }
    }
    return beg;
}

KP::KP() {

}
bool KP::operator<(const KP &v) const {
    bool t = comparemas(this->key, v.key);
    return t;
}
