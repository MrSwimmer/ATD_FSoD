//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H

#include <list>
#include "iostream"
#include "KP.h"

using namespace std;
class FSD {

    const char* INDEX_FILE_NAME = "index.txt";
    const char* NOTES_FILE_NAME = "notes.txt";
    int COUNT_BLOCKS=1;
    int COUNT_NOTES_IN_BLOCK;

public:
    FSD();
    void insert(char *ikey, void *iNote);
    long long int addToEnd(void *iNote);
    long long getBeginBlock(char *key);
    void *getNote(char *ikey);
    void *getNoteSup(long long int point);
    void delNote(char *key);
    long long int getLocalNoteIndex(char *ikey, KP kpr, int c, int wh);
    void getall(std::__cxx11::list<void *> notes);
    void moveNotes(int c, long long int placeInsert);

    void getall(list<char *> notes);

    void getall();

    FSD(int itemsInBlock);

    long long int getLocalNoteIndex(char *ikey);
};

#endif //ATD_FSOD_FSD_H
