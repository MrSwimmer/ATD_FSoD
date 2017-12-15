//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H

#include "iostream"
#include "KP.h"

using namespace std;
class FSD {
    char* INDEX_FILE_NAME = "index.txt";
    char *NOTES_FILE_NAME = "notes.txt";
    int COUNT_BLOCKS=8;
    int COUNT_NOTES_IN_BLOCK=1000;
    int COUNT_SYMBOLS_IN=255/COUNT_BLOCKS;

public:
    void insert(char *ikey, void *iNote);
    FSD();
    void getall();
    long long int addToEnd(void *iNote);
    long long getBeginBlock(char *key);
    void *getNote(char *ikey);
    void *getNoteSup(long long int point);
    void delNote(char *key);

    void moveNotes(int c, long long int placeInsert, ofstream iout);

    long long int getLocalNoteIndex(char *ikey, KP kpr, int c, int wh);
};

#endif //ATD_FSOD_FSD_H
