#include "ArrayCollection.h"
#include <iostream>

bool ArrayCollection::insertOne(const Document& doc) {
    for (const auto& d : documents) {
        if (d.id == doc.id) return false;
    }
    documents.push_back(doc);
    return true;
}

bool ArrayCollection::deleteOne(int id) {
    for (size_t i = 0; i < documents.size(); i++) {
        if (documents[i].id == id) {
            documents.erase(documents.begin() + i);
            return true;
        }
    }
    return false;
}

Document* ArrayCollection::findById(int id) {
    for (auto& doc : documents) {
        if (doc.id == id) return &doc;
    }
    return nullptr;
}

std::vector<Document> ArrayCollection::findAll() {
    return documents;
}

std::vector<Document> ArrayCollection::filter(const std::string& field, const std::string& value) {
    std::vector<Document> result;
    if (field == "name") {
        for (const auto& doc : documents) {
            if (doc.name == value) result.push_back(doc);
        }
    } else if (field == "id") {
        int idVal = std::stoi(value);
        for (const auto& doc : documents) {
            if (doc.id == idVal) result.push_back(doc);
        }
    }
    return result;
}

int ArrayCollection::count() {
    return documents.size();
}

double ArrayCollection::sum(const std::string& field) {
    if (field != "gpa") return 0.0;
    double total = 0.0;
    for (const auto& doc : documents) total += doc.gpa;
    return total;
}

double ArrayCollection::average(const std::string& field) {
    if (field != "gpa" || documents.empty()) return 0.0;
    return sum(field) / documents.size();
}

Collection* ArrayCollection::clone() const {
    auto* newCollection = new ArrayCollection();
    newCollection->documents = this->documents;
    return newCollection;
}
