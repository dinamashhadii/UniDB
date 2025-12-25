#pragma once
#include "Collection.h"

// یک نود ساده برای لیست پیوندی
struct Node {
    Document doc;
    Node* next;
    Node(const Document& d) : doc(d), next(nullptr) {}
};

// پیاده‌سازی لیست پیوندی برای ذخیره Document ها
class LinkedListCollection : public Collection {
private:
    Node* head;

public:
    LinkedListCollection() : head(nullptr) {}
    ~LinkedListCollection();

    bool insertOne(const Document& doc) override;
    bool deleteOne(int id) override;
    Document* findById(int id) override;
    std::vector<Document> findAll() override;
    std::vector<Document> filter(const std::string& field, const std::string& value) override;
    int count() override;
    double sum(const std::string& field) override;
    double average(const std::string& field) override;
    Collection* clone() const override;

private:
    void clear();
};
