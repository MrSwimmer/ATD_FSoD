//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H

#include "iostream"
#include "KP.h"

using namespace std;
class FSD {
    const char* INDEX_FILE_NAME = "index.txt";
    const char* NOTES_FILE_NAME = "notes.txt";
    int COUNT_BLOCKS=8;
    int COUNT_NOTES_IN_BLOCK=1000;
    int COUNT_SYMBOLS_IN=255/COUNT_BLOCKS;

public:
    void insert(char *ikey, void *iNote);
    FSD();
    void getall();
    long long int addToEnd(void *iNote);
    long long getBeginBlock(char *key);
    long long int getLocalNote(char *ikey, KP kpr, int c);

    void getNote(char *ikey);

    void getNote(char *ikey, void *buf);
};

#endif //ATD_FSOD_FSD_H
