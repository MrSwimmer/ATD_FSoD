//
// Created by Севастьян on 03.12.2017.
//

#ifndef ATD_FSOD_INTERBASE_H
#define ATD_FSOD_INTERBASE_H


class InterBase
{
public:
    virtual int size_of_data() = 0;
    /*Возвращает указатель на данные. Память под этот указатель будет освобождена
    в функции insert_item класса FileBase. Т.е. память под этот указатель должна
    быть выделена динамически, и все данные, которые нужно скопировать в файл,
    должны быть перенесены в выделенную память.*/
    virtual void* get_data() = 0;
};


#endif //ATD_FSOD_INTERBASE_H
