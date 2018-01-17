//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_KP_H
#define ATD_FSOD_KP_H
#include "FSD.h"
class KP {
    //const lenght = 4;
public:
    KP(char* key, int pointer) {
        this->key = key;
        this->pointer = pointer;
    }
    bool operator < (const KP &v) const ;
    KP();
    int pointer;
    char *key;
};
#endif //ATD_FSOD_KP_H
