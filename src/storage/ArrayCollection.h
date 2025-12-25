#ifndef ARRAYCOLLECTION_H
#define ARRAYCOLLECTION_H

#include "Collection.h"
#include <vector>
#include <algorithm>

class ArrayCollection : public Collection {
private:
    std::vector<Document> documents;

public:
    bool insertOne(const Document& doc) override {
        for (const auto& document : documents) {
            if (document.id == doc.id) {
                return false;
            }
        }
        documents.push_back(doc);
        return true;
    }

    bool deleteOne(int id) override {
        for (size_t i = 0; i < documents.size(); i++) {
            if (documents[i].id == id) {
                documents.erase(documents.begin() + i);
                return true;
            }
        }
        return false;
    }

    Document* findById(int id) override {
        for (auto& doc : documents) {
            if (doc.id == id) {
                return &doc;
            }
        }
        return nullptr;
    }

    std::vector<Document> findAll() override {
        return documents;
    }

    std::vector<Document> filter(const std::string& field, const std::string& value) override {
        std::vector<Document> result;

        if (field == "name") {
            for (const auto& doc : documents) {
                if (doc.name == value) {
                    result.push_back(doc);
                }
            }
        } else if (field == "id") {
            int idValue = std::stoi(value);
            for (const auto& doc : documents) {
                if (doc.id == idValue) {
                    result.push_back(doc);
                }
            }
        } else if (field == "gpa") {
            double gpaValue = std::stod(value);
            for (const auto& doc : documents) {
                if (doc.gpa == gpaValue) {
                    result.push_back(doc);
                }
            }
        }

        return result;
    }

    int count() override {
        return documents.size();
    }

    double sum(const std::string& field) override {
        if (field != "gpa") return 0.0;

        double total = 0.0;
        for (const auto& doc : documents) {
            total += doc.gpa;
        }
        return total;
    }

    double average(const std::string& field) override {
        if (field != "gpa" || documents.empty()) return 0.0;
        return sum(field) / documents.size();
    }

    Collection* clone() const override {
        ArrayCollection* newCollection = new ArrayCollection();
        newCollection->documents = this->documents;
        return newCollection;
    }
};

#endif // ARRAYCOLLECTION_H