#ifndef LINKEDLISTCOLLECTION_H
#define LINKEDLISTCOLLECTION_H

#include "Collection.h"

struct Node {
    Document data;
    Node* prev;
    Node* next;

    Node(const Document& doc) : data(doc), prev(nullptr), next(nullptr) {}
};

class LinkedListCollection : public Collection {
private:
    Node* head;
    Node* tail;
    int size;

    Node* findNodeById(int id) const {
        Node* current = head;
        while (current) {
            if (current->data.id == id) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    void deleteNode(Node* node) {
        if (!node) return;

        if (node->prev) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }

        if (node->next) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }

        delete node;
    }

public:
    LinkedListCollection() : head(nullptr), tail(nullptr), size(0) {}

    ~LinkedListCollection() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    bool insertOne(const Document& doc) override {
        if (findNodeById(doc.id) != nullptr) {
            return false;
        }

        Node* newNode = new Node(doc);

        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        size++;
        return true;
    }

    bool deleteOne(int id) override {
        Node* nodeToDelete = findNodeById(id);
        if (!nodeToDelete) {
            return false;
        }

        deleteNode(nodeToDelete);
        size--;
        return true;
    }

    Document* findById(int id) override {
        Node* node = findNodeById(id);
        return node ? &(node->data) : nullptr;
    }

    std::vector<Document> findAll() override {
        std::vector<Document> result;
        Node* current = head;

        while (current) {
            result.push_back(current->data);
            current = current->next;
        }

        return result;
    }

    std::vector<Document> filter(const std::string& field, const std::string& value) override {
        std::vector<Document> result;
        Node* current = head;

        while (current) {
            if (field == "name" && current->data.name == value) {
                result.push_back(current->data);
            } else if (field == "id" && std::to_string(current->data.id) == value) {
                result.push_back(current->data);
            } else if (field == "gpa" && std::to_string(current->data.gpa) == value) {
                result.push_back(current->data);
            }
            current = current->next;
        }

        return result;
    }

    int count() override {
        return size;
    }

    double sum(const std::string& field) override {
        if (field != "gpa") return 0.0;

        double total = 0.0;
        Node* current = head;

        while (current) {
            total += current->data.gpa;
            current = current->next;
        }

        return total;
    }

    double average(const std::string& field) override {
        if (field != "gpa" || size == 0) return 0.0;
        return sum(field) / size;
    }

    Collection* clone() const override {
        LinkedListCollection* newCollection = new LinkedListCollection();
        Node* current = head;

        while (current) {
            newCollection->insertOne(current->data);
            current = current->next;
        }

        return newCollection;
    }
};

#endif // LINKEDLISTCOLLECTION_H