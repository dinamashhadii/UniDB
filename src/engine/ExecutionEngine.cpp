#include "ExecutionEngine.h"
#include <sstream>

ExecutionEngine::ExecutionEngine() {
    collection = std::make_unique<ArrayCollection>();
}

ExecutionEngine::~ExecutionEngine() {
    transactionStack.clear();
}

std::string ExecutionEngine::execute(const Command& cmd) {
    if (inBatchMode && cmd.type != CommandType::BATCH_EXECUTE) {
        batchQueue.push(cmd);
        return "Command added to batch queue";
    }

    switch (cmd.type) {
        case CommandType::BEGIN_TRANSACTION:
            if (inTransaction) return "ERROR: Transaction already in progress";
            transactionStack.push(collection->clone());
            inTransaction = true;
            return "Transaction started";

        case CommandType::COMMIT:
            if (!inTransaction) return "ERROR: No transaction in progress";
            transactionStack.clear();
            inTransaction = false;
            return "Transaction committed";

        case CommandType::ROLLBACK:
            if (!inTransaction || transactionStack.empty()) return "ERROR: No transaction to rollback";
            delete collection.release();
            collection.reset(transactionStack.top());
            transactionStack.pop();
            inTransaction = false;
            return "Transaction rolled back";

        case CommandType::BATCH_START:
            if (inBatchMode) return "ERROR: Batch mode already active";
            inBatchMode = true;
            return "Batch mode started";

        case CommandType::BATCH_EXECUTE: {
            if (!inBatchMode) return "ERROR: Batch mode not active";
            int executed = 0;
            while (!batchQueue.empty()) {
                Command batchCmd = batchQueue.front();
                executeSingleCommand(batchCmd);
                batchQueue.pop();
                executed++;
            }
            inBatchMode = false;
            return "Batch executed: " + std::to_string(executed) + " operations completed";
        }

        default:
            return executeSingleCommand(cmd);
    }
}

std::string ExecutionEngine::executeSingleCommand(const Command& cmd) {
    switch (cmd.type) {
        case CommandType::INSERT_ONE:
            return collection->insertOne(cmd.document) ?
                "SUCCESS: Document inserted with ID " + std::to_string(cmd.document.id) :
                "ERROR: Document with ID " + std::to_string(cmd.document.id) + " already exists";

        case CommandType::DELETE_ONE:
            return collection->deleteOne(cmd.document.id) ?
                "SUCCESS: Document with ID " + std::to_string(cmd.document.id) + " deleted" :
                "ERROR: Document with ID " + std::to_string(cmd.document.id) + " not found";

        case CommandType::FIND_BY_ID: {
            Document* doc = collection->findById(cmd.document.id);
            return doc ?
                "ID: " + std::to_string(doc->id) + ", Name: " + doc->name + ", GPA: " + std::to_string(doc->gpa) :
                "ERROR: Document with ID " + std::to_string(cmd.document.id) + " not found";
        }

        case CommandType::FIND_ALL: {
            auto docs = collection->findAll();
            if (docs.empty()) return "No documents found";
            std::ostringstream result;
            int counter = 1;
            for (const auto& doc : docs) {
                result << counter++ << ". ID: " << doc.id
                       << ", Name: " << doc.name
                       << ", GPA: " << doc.gpa << "\n";
            }
            return result.str();
        }

        case CommandType::FILTER: {
            auto docs = collection->filter(cmd.field, cmd.value);
            if (docs.empty()) return "No documents match the criteria";
            std::ostringstream result;
            result << "Found " << docs.size() << " document(s):\n";
            int counter = 1;
            for (const auto& doc : docs) {
                result << counter++ << ". ID: " << doc.id
                       << ", Name: " << doc.name
                       << ", GPA: " << doc.gpa << "\n";
            }
            return result.str();
        }

        case CommandType::COUNT:
            return "Total documents: " + std::to_string(collection->count());

        case CommandType::SUM:
            return "Sum of " + cmd.field + ": " + std::to_string(collection->sum(cmd.field));

        case CommandType::AVERAGE:
            return "Average " + cmd.field + ": " + std::to_string(collection->average(cmd.field));

        default:
            return "ERROR: Unknown command type";
    }
}
