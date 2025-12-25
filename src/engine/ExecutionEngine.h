#ifndef EXECUTIONENGINE_H
#define EXECUTIONENGINE_H

#include "Command.h"
#include "../storage/ArrayCollection.h"
#include <memory>
#include <stack>
#include <queue>
#include <string>
#include <sstream>

class ExecutionEngine {
private:
    std::unique_ptr<Collection> collection;
    std::stack<Collection*> transactionStack;
    std::queue<Command> batchQueue;
    bool inTransaction;
    bool inBatchMode;

    std::string executeInsertOne(const Command& cmd) {
        if (collection->insertOne(cmd.document)) {
            return "SUCCESS: Document inserted with ID " + std::to_string(cmd.document.id);
        }
        return "ERROR: Document with ID " + std::to_string(cmd.document.id) + " already exists";
    }

    std::string executeDeleteOne(const Command& cmd) {
        if (collection->deleteOne(cmd.document.id)) {
            return "SUCCESS: Document with ID " + std::to_string(cmd.document.id) + " deleted";
        }
        return "ERROR: Document with ID " + std::to_string(cmd.document.id) + " not found";
    }

    std::string executeFindById(const Command& cmd) {
        Document* doc = collection->findById(cmd.document.id);
        if (doc) {
            std::stringstream ss;
            ss << "ID: " << doc->id
               << ", Name: " << doc->name
               << ", GPA: " << doc->gpa;
            return ss.str();
        }
        return "ERROR: Document with ID " + std::to_string(cmd.document.id) + " not found";
    }

    std::string executeFindAll() {
        std::vector<Document> docs = collection->findAll();
        if (docs.empty()) {
            return "No documents found";
        }

        std::stringstream ss;
        int counter = 1;
        for (const auto& doc : docs) {
            ss << counter++ << ". ID: " << doc.id
               << ", Name: " << doc.name
               << ", GPA: " << doc.gpa << "\n";
        }
        return ss.str();
    }

    std::string executeFilter(const Command& cmd) {
        std::vector<Document> docs = collection->filter(cmd.field, cmd.value);
        if (docs.empty()) {
            return "No documents match the criteria";
        }

        std::stringstream ss;
        ss << "Found " << docs.size() << " document(s):\n";
        int counter = 1;
        for (const auto& doc : docs) {
            ss << counter++ << ". ID: " << doc.id
               << ", Name: " << doc.name
               << ", GPA: " << doc.gpa << "\n";
        }
        return ss.str();
    }

    std::string executeCount() {
        return "Total documents: " + std::to_string(collection->count());
    }

    std::string executeSum(const Command& cmd) {
        return "Sum of " + cmd.field + ": " + std::to_string(collection->sum(cmd.field));
    }

    std::string executeAverage(const Command& cmd) {
        return "Average " + cmd.field + ": " + std::to_string(collection->average(cmd.field));
    }

public:
    ExecutionEngine() : inTransaction(false), inBatchMode(false) {
        collection = std::make_unique<ArrayCollection>();
    }

    ~ExecutionEngine() {
        while (!transactionStack.empty()) {
            delete transactionStack.top();
            transactionStack.pop();
        }
    }

    std::string execute(const Command& cmd) {
        if (inBatchMode && cmd.type != CommandType::BATCH_EXECUTE) {
            batchQueue.push(cmd);
            return "Command added to batch queue";
        }

        if (cmd.type == CommandType::BEGIN_TRANSACTION) {
            if (inTransaction) {
                return "ERROR: Transaction already in progress";
            }
            transactionStack.push(collection->clone());
            inTransaction = true;
            return "Transaction started";
        }
        else if (cmd.type == CommandType::COMMIT) {
            if (!inTransaction) {
                return "ERROR: No transaction in progress";
            }
            while (!transactionStack.empty()) {
                delete transactionStack.top();
                transactionStack.pop();
            }
            inTransaction = false;
            return "Transaction committed";
        }
        else if (cmd.type == CommandType::ROLLBACK) {
            if (!inTransaction || transactionStack.empty()) {
                return "ERROR: No transaction to rollback";
            }
            delete collection.release();
            collection.reset(transactionStack.top());
            transactionStack.pop();
            inTransaction = false;
            return "Transaction rolled back";
        }
        else if (cmd.type == CommandType::BATCH_START) {
            if (inBatchMode) {
                return "ERROR: Batch mode already active";
            }
            inBatchMode = true;
            return "Batch mode started";
        }
        else if (cmd.type == CommandType::BATCH_EXECUTE) {
            if (!inBatchMode) {
                return "ERROR: Batch mode not active";
            }

            int executed = 0;
            while (!batchQueue.empty()) {
                Command batchCmd = batchQueue.front();
                execute(batchCmd);
                batchQueue.pop();
                executed++;
            }

            inBatchMode = false;
            return "Batch executed: " + std::to_string(executed) + " operations completed";
        }

        switch (cmd.type) {
            case CommandType::INSERT_ONE:
                return executeInsertOne(cmd);
            case CommandType::DELETE_ONE:
                return executeDeleteOne(cmd);
            case CommandType::FIND_BY_ID:
                return executeFindById(cmd);
            case CommandType::FIND_ALL:
                return executeFindAll();
            case CommandType::FILTER:
                return executeFilter(cmd);
            case CommandType::COUNT:
                return executeCount();
            case CommandType::SUM:
                return executeSum(cmd);
            case CommandType::AVERAGE:
                return executeAverage(cmd);
            default:
                return "ERROR: Unknown command type";
        }
    }
};

#endif // EXECUTIONENGINE_H