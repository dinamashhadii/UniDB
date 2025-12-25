#pragma once
#include "Collection.h"
#include <algorithm>

class ArrayCollection : public Collection {
private:
    std::vector<Document> documents;

public:
    bool insertOne(const Document& doc) override;
    bool deleteOne(int id) override;
    Document* findById(int id) override;
    std::vector<Document> findAll() override;
    std::vector<Document> filter(const std::string& field, const std::string& value) override;
    int count() override;
    double sum(const std::string& field) override;
    double average(const std::string& field) override;
    Collection* clone() const override;
};
