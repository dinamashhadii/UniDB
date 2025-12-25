#pragma once
#include <memory>
#include <queue>
#include "storage/ArrayCollection.h"
#include "engine/Command.h"
#include "engine/TransactionStack.h"

class ExecutionEngine {
private:
    std::unique_ptr<Collection> collection;
    TransactionStack transactionStack;
    std::queue<Command> batchQueue;
    bool inTransaction = false;
    bool inBatchMode = false;

public:
    ExecutionEngine();
    ~ExecutionEngine();
    std::string execute(const Command& cmd);

private:
    std::string executeSingleCommand(const Command& cmd);
};
