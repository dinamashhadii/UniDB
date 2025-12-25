#pragma once
#include <stack>
#include <memory>
#include "storage/Collection.h"

class TransactionStack {
private:
    std::stack<std::unique_ptr<Collection>> stack;

public:
    void push(std::unique_ptr<Collection> c) {
        stack.push(std::move(c));
    }

    bool empty() const { return stack.empty(); }

    Collection* top() { return stack.top().get(); }

    void pop() { stack.pop(); }

    void clear() {
        while (!stack.empty()) {
            stack.pop();
        }
    }
    std::unique_ptr<Collection> popAndTake() {
        auto top = std::move(stack.top());
        stack.pop();
        return top;
    }
};