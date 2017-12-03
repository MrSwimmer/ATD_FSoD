//
// Created by Севастьян on 03.12.2017.
//

#ifndef ATD_FSOD_NOTE_H
#define ATD_FSOD_NOTE_H

class Note {

public:
    Note() {
    }
    Note(std::string note) {
        this->note = note;
        this->lenght = note.length();
    }

    int lenght;
    std::string note;

};

#endif //ATD_FSOD_NOTE_H
