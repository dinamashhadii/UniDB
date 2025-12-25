#ifndef COLLECTION_H
#define COLLECTION_H

#include "../models/Document.h"
#include <vector>
#include <string>

class Collection {
public:
    virtual ~Collection() = default;

    virtual bool insertOne(const Document& doc) = 0;
    virtual bool deleteOne(int id) = 0;
    virtual Document* findById(int id) = 0;
    virtual std::vector<Document> findAll() = 0;
    virtual std::vector<Document> filter(const std::string& field, const std::string& value) = 0;
    virtual int count() = 0;
    virtual double sum(const std::string& field) = 0;
    virtual double average(const std::string& field) = 0;
    virtual Collection* clone() const = 0;
};

#endif