//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H

#include "iostream"
using namespace std;
class FSD {
    const char* INDEX_FILE_NAME = "index.txt";
    const char* NOTES_FILE_NAME = "notes.txt";
    int COUNT_BLOCKS=5;
    int COUNT_NOTES_IN_BLOCK=100;

public:
    /*void insert(string ikey, void *iNote, int size);
    void getnote(char *dest, string key);*/
    void insert(char *ikey, void *iNote, int size);
    void getnote(char *dest, char *key);
    FSD();


};

#endif //ATD_FSOD_FSD_H
