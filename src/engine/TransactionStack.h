#pragma once
#include <stack>
#include "storage/Collection.h"

class TransactionStack {
private:
    std::stack<Collection*> stack;

public:
    void push(Collection* c) { stack.push(c); }
    bool empty() const { return stack.empty(); }
    Collection* top() { return stack.top(); }
    void pop() { stack.pop(); }
    void clear() {
        while (!stack.empty()) {
            delete stack.top();
            stack.pop();
        }
    }
};
