//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_KP_H
#define ATD_FSOD_KP_H

class KP {
public:
    KP(std::string key, int pointer) {
        this->key = key;
        this->pointer = pointer;
    }

    KP() {

    }

    int pointer;
    std::string key;
};


#endif //ATD_FSOD_KP_H
