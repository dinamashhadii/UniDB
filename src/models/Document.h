#pragma once
#include <string>

struct Document {
    int id;
    std::string name;
    double gpa;

    Document(int i = 0, std::string n = "", double g = 0.0)
        : id(i), name(n), gpa(g) {}
};
