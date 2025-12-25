#ifndef TRANSACTIONSTACK_H
#define TRANSACTIONSTACK_H

#include "../storage/Collection.h"
#include <stack>
#include <memory>

class TransactionStack {
private:
    std::stack<Collection*> stack;

public:
    TransactionStack() = default;

    ~TransactionStack() {
        clear();
    }
    void push(Collection* collection) {
        if (collection) {
            stack.push(collection->clone());
        }
    }
    Collection* pop() {
        if (stack.empty()) {
            return nullptr;
        }

        Collection* topCollection = stack.top();
        stack.pop();
        return topCollection;
    }
    Collection* top() const {
        if (stack.empty()) {
            return nullptr;
        }
        return stack.top();
    }
    bool isEmpty() const {
        return stack.empty();
    }
    size_t size() const {
        return stack.size();
    }
    void clear() {
        while (!stack.empty()) {
            Collection* collection = stack.top();
            delete collection;
            stack.pop();
        }
    }
    TransactionStack(const TransactionStack& other) {
        std::stack<Collection*> tempStack;
        std::stack<Collection*> originalStack = other.stack;

        while (!originalStack.empty()) {
            tempStack.push(originalStack.top()->clone());
            originalStack.pop();
        }
        while (!tempStack.empty()) {
            stack.push(tempStack.top());
            tempStack.pop();
        }
    }
    TransactionStack& operator=(const TransactionStack& other) {
        if (this != &other) {
            // Clear current stack
            clear();

            std::stack<Collection*> tempStack;
            std::stack<Collection*> originalStack = other.stack;

            while (!originalStack.empty()) {
                tempStack.push(originalStack.top()->clone());
                originalStack.pop();
            }
            while (!tempStack.empty()) {
                stack.push(tempStack.top());
                tempStack.pop();
            }
        }
        return *this;
    }
    TransactionStack(TransactionStack&& other) noexcept {
        stack = std::move(other.stack);
    }
    TransactionStack& operator=(TransactionStack&& other) noexcept {
        if (this != &other) {
            clear();
            stack = std::move(other.stack);
        }
        return *this;
    }
};

#endif