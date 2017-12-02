//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H


class FSD {
    const char* INDEX_FILE_NAME = "index";
    const char* NOTES_FILE_NAME = "notes.txt";

public:

    void insert(std::string ikey, std::string inote);
};


#endif //ATD_FSOD_FSD_H
