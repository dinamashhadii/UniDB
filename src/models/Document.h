#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

struct Document {
    int id;
    std::string name;
    double gpa;

    Document(int _id = 0, std::string _name = "", double _gpa = 0.0)
        : id(_id), name(_name), gpa(_gpa) {}
};

#endif