//
// Created by Севастьян on 02.12.2017.
//

#ifndef ATD_FSOD_FSD_H
#define ATD_FSOD_FSD_H

#include "Note.h"
#include "InterBase.h"

typedef long long adress;
using namespace std;
class FSD {


public:
    FSD();

    const char* INDEX_FILE_NAME = "index.txt";
    const char* NOTES_FILE_NAME = "notes.txt";

    FSD(const char* name, int idx_count = 10, bool locked = true);
    ~FSD();
    void open(const char* name, int idx_count = 10, bool locked = true);
    //void insert(string ikey, void* iNote, int size);
    /*Память указателя, возвращаемого методом p->get_data, будет освобождена в функции.
	Возвращает ключ key добавленной записи.*/
    float insert_item(InterBase* p);
    /*Побитовое копирование данных указателя void* data размером size байт в файл.
    Возвращает ключ key добавленной записи.*/
    float insert_item(void* data, int size);
    /*Поиск записи по ключу. Возвращает указатель на положение элемента в файле.*/
    std::streampos search_item(float key);
    /*Удаляет запись по ключу.*/
    void delete_item(float key);
    /*Записывает элемент, находящийся в файле на позиции pos, в память.
    Указатель dest будет указывать на выгруженный элемент.*/
    void get_item(void*& dest, std::streampos pos);
    /*Стирание удаленных записей из базы данных. Не соответсвует принципам закреплённых записей.*/
    void compress();

private:
    /*Вставка вместо удаленной записи в файле индексов. Используется в функции insert_item только при условии,
    что записи не закреплённые.*/
    float insert_on_deleted(void* data, int size);

    float* get_item(int item);
    void flush_block(int block);
    //void flush_block(std::streampos pos);
    void get_block(int block);
    void get_block(std::streampos pos);


    char* name;
    std::fstream base;
    std::fstream idx;

    bool is_locked;

    adress beg;
    adress tail;

    char* buf;
    int item_size;
    int block_size;
    int cur_block;

    int count;
    float max_key;

    void insert(string ikey, void *iNote, int size);
};

#endif //ATD_FSOD_FSD_H
