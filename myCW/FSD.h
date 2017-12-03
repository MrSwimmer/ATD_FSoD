//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H

#include "Note.h"

using namespace std;
class FSD {
    const char* INDEX_FILE_NAME = "index.txt";
    const char* NOTES_FILE_NAME = "notes.txt";

public:
    void insert(string ikey, void* iNote, int size);
};

#endif //ATD_FSOD_FSD_H
