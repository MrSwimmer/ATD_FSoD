//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H

#include <list>
#include <set>
#include "iostream"
#include "KP.h"


using namespace std;
class FSD {

    const char* INDEX_FILE_NAME = "index.txt";
    const char* NOTES_FILE_NAME = "notes.txt";
    const char* EMP_FILE_NAME = "emp.txt";
    int COUNT_BLOCKS=1;
    int COUNT_NOTES_IN_BLOCK;
    int sizehalf;
    set<KP> setblocks;
    char* emp;
    int c=0;
public:
    FSD(int itemsInBlock);
    void insert(char *ikey, void *iNote);
    long long int addToEnd(void *iNote);
    long long getBeginBlock(char *key);
    void *getNote(char *ikey);
    void *getNoteSup(long long int point);
    void delNote(char *key);
    void getall();

    long long int getLocalNoteInBlock(char *key, long long int beginblock);
};

#endif //ATD_FSOD_FSD_H
