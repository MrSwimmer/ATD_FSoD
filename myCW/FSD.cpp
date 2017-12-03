//
// Created by Севастьян on 02.12.2017.
//
#include <fstream>
#include "FSD.h"
#include "KP.h"
#include "Note.h"
#include <iostream>

using namespace std;

FSD::FSD(const char *name, int idx_count, bool locked):FSD() {
    open(name, idx_count, locked);
}

FSD::~FSD() {
    flush_block(cur_block);
    if (!is_locked)
        compress();
    if (base)
        base.close();
    if (idx)
        idx.close();
}

FSD::FSD() :max_key(0), beg(0), tail(0), item_size(0), block_size(0), cur_block(-1), buf(nullptr), is_locked(true) {

}

void FSD::open(const char *name, int idx_count, bool locked) {

}

float FSD::insert_item(InterBase *p) {
    void* data = p->get_data();
    float result = insert_item(data, p->size_of_data());
    delete[] data;
    return result;
}

float FSD::insert_item(void *data, int size) {
    /*Нулевой ключ зарезервирован для пустых записей.*/
    max_key++;


    if (max_key == 1)
    {
        idx.write((char*)&count, sizeof(int));
        idx.write((char*)&beg, sizeof(adress));
        idx.write((char*)&tail, sizeof(adress));
        //idx.flush();
    }


    /*Размер элемента.*/
    //int item_size = sizeof(float) + sizeof(adress);
    //int block_size = item_size*count;
    /*В каком блоке находится элемент.*/
    int block_pos = (max_key - 1) / count;
    /*Позиция блока.*/
    std::streampos p(sizeof(int) + 2 * sizeof(adress) + block_pos*block_size);
    if (!is_locked) {
        float result = insert_on_deleted(data, size);
        if (result != 0)
            return result;
    }

    if (block_pos != cur_block) {
        if (cur_block != -1) {
            flush_block(cur_block);
        }
        /*Для выяснения, будет ли это новый блок.*/
        idx.seekg(0, std::ios::end);
        std::streampos pp = idx.tellg();
        /*Если этот блок будет новым.*/
        if (p == pp) {
            int i = 0;
            while (i < block_size)
                buf[i++] = 0;
        }
        else {
            get_block(p);
        }
        cur_block = block_pos;
    }
    /*Куда будет записываться новые ключ и адрес.*/
    float* item = get_item(((int)max_key - 1) % count);//(float*)(buf + ((((int)max_key - 1) % count)*item_size));
    *item = max_key;
    item++;

    /*Запись в базу данных и сохранение индекса.*/
    base.write((char*)&size, sizeof(int));
    *((adress*)item) = base.tellp();
    base.write((char*)data, size);
    /*Запись блока.*/
    //idx.seekp(p);
    //idx.write(buf, block_size);

    //delete buf;

    return max_key;
}

std::streampos FSD::search_item(float key) {

}


