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

    // Push a copy of collection onto stack
    void push(Collection* collection) {
        if (collection) {
            stack.push(collection->clone());
        }
    }

    // Pop and return top collection
    Collection* pop() {
        if (stack.empty()) {
            return nullptr;
        }

        Collection* topCollection = stack.top();
        stack.pop();
        return topCollection;
    }

    // Get top collection without removing it
    Collection* top() const {
        if (stack.empty()) {
            return nullptr;
        }
        return stack.top();
    }

    // Check if stack is empty
    bool isEmpty() const {
        return stack.empty();
    }

    // Get stack size
    size_t size() const {
        return stack.size();
    }

    // Clear all collections from stack
    void clear() {
        while (!stack.empty()) {
            Collection* collection = stack.top();
            delete collection;
            stack.pop();
        }
    }

    // Copy constructor (deep copy)
    TransactionStack(const TransactionStack& other) {
        // Create a temporary stack to reverse order
        std::stack<Collection*> tempStack;
        std::stack<Collection*> originalStack = other.stack;

        while (!originalStack.empty()) {
            tempStack.push(originalStack.top()->clone());
            originalStack.pop();
        }

        // Push back in correct order
        while (!tempStack.empty()) {
            stack.push(tempStack.top());
            tempStack.pop();
        }
    }

    // Assignment operator (deep copy)
    TransactionStack& operator=(const TransactionStack& other) {
        if (this != &other) {
            // Clear current stack
            clear();

            // Create a temporary stack to reverse order
            std::stack<Collection*> tempStack;
            std::stack<Collection*> originalStack = other.stack;

            while (!originalStack.empty()) {
                tempStack.push(originalStack.top()->clone());
                originalStack.pop();
            }

            // Push back in correct order
            while (!tempStack.empty()) {
                stack.push(tempStack.top());
                tempStack.pop();
            }
        }
        return *this;
    }

    // Move constructor
    TransactionStack(TransactionStack&& other) noexcept {
        stack = std::move(other.stack);
    }

    // Move assignment operator
    TransactionStack& operator=(TransactionStack&& other) noexcept {
        if (this != &other) {
            clear();
            stack = std::move(other.stack);
        }
        return *this;
    }
};

#endif // TRANSACTIONSTACK_H