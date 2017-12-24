//
// Created by Севастьян on 24.12.2017.
//

#ifndef ATD_FSOD_BLOCK_H
#define ATD_FSOD_BLOCK_H


class Block {
    char *key;
    long long pointer;
public:
    Block(char *key, long long int pointer) {
        this->key = key;
        this->pointer = pointer;
    }

    bool comparemas(char *a, char *b) {
        if(b=="_"){
            return true;
        } else {
            return a[0] < b[0];
        }
    }

    bool operator < (Block block) {
        return comparemas(this->key, block.key);
    }
    bool operator > (Block block) {
        return !comparemas(this->key, block.key);
    }
};

bool Student::operator < (Student student)
{
    if(this->age<student.age){
        return true;
    } else {
        return false;
    }
}
bool Student::operator > (Student student)
{
    if(this->age<student.age){
        return false;
    } else {
        return true;
    }
}
#endif //ATD_FSOD_BLOCK_H
