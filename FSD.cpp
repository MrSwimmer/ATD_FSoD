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
    if (FileExists(INDEX_FILE_NAME)) {
        cout << "YUP" << endl;
    } else {
        cout << "NOPE";
        fstream femp(EMP_FILE_NAME, ios::binary | ios::app | ios::out | ios::in);
        for (int i = 0; i < itemsInBlock/2; i++) {
            KP kp(const_cast<char *>("_"), 1);
            femp.write((char *)&kp, sizeof(KP));
        }
        femp.seekg(0);
        femp.read(emp, sizehalf);
        femp.close();
        fstream fin(INDEX_FILE_NAME, ios::binary | ios::out);
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
    long long local = getLocalNoteInBlock(ikey, begin);//номер записи
    long long localins = (local + COUNT_NOTES_IN_BLOCK * begin) * sizeof(KP);//указатель на запись
    long long lastnote = (COUNT_NOTES_IN_BLOCK * (begin + 1) - 1) * sizeof(KP);
    long long middle = (COUNT_NOTES_IN_BLOCK / 2 + COUNT_NOTES_IN_BLOCK * begin) * sizeof(KP);
    fstream in(INDEX_FILE_NAME, ios::binary | ios::in | ios::out);

    in.seekg(localins);
    int sizerest = static_cast<int>((COUNT_NOTES_IN_BLOCK - local - 1) * sizeof(KP));
    char *rest = new char[sizerest];
    in.read(rest, sizerest);
    in.seekp(localins);
    KP kp(ikey, static_cast<int>(point));
    in.write((char *) &kp, sizeof(KP));
    in.write(rest, sizerest);
    KP last;
    in.seekg(lastnote);
    in.read((char *) &last, sizeof(KP));
    if (last.key != "_") {
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
        KP kpnb(beginbl.key, begnewbl);
        setblocks.insert(kpnb);
    }
    in.close();
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
    int div = 2;
    long long beg = 0;
    int size = setblocks.size();
    int loc = size / div;
    while (true) {
        int c = 0;
        char *keyset;
        for (auto i : setblocks) {
            if (c == loc) {
                keyset = i.key;
                break;
            }
            c++;
        }
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
    if (beg < 0) {
        beg = 0;
    }
    return beg;
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
        int f = (loc + beginblock * COUNT_NOTES_IN_BLOCK) * sizeof(KP);
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
