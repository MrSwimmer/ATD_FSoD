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
    int COUNT_NOTES_IN_BLOCK;
    int sizehalf;
    set<KP> setblocks;
public:

private:
    char* emp;
    int c=0;
public:
    FSD(int itemsInBlock);
    int count = 0;
    int COUNT_BLOCKS=1;
    void insert(char *ikey, void *iNote);
    long long int addToEnd(void *iNote);
    long long getBeginBlock(char *key);
    void getall();
    long long int getLocalNoteInBlock(char *key, long long int beginblock);
    void del(char *ikey);
    void find(char *ikey);
    void saveset();
    void getset();
    void getSetFromFile();

    long long int getLocalNoteInBlockToSearch(char *key, long long int beginblock);
};

#endif //ATD_FSOD_FSD_H
