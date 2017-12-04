//
// Created by Севастьян on 04.12.2017.
//

#ifndef ATD_FSOD_INDEXINFO_H
#define ATD_FSOD_INDEXINFO_H


class IndexInfo {
    char* param;
    int count;
public:
    IndexInfo(char *param, int count) {
        this->count = count;
        this->param = param;
    }

    IndexInfo() {}
};


#endif //ATD_FSOD_INDEXINFO_H
