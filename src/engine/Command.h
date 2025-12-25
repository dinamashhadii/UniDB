#pragma once
#include <string>
#include "models/Document.h"

enum class CommandType {
    INSERT_ONE, DELETE_ONE, FIND_BY_ID, FIND_ALL, FILTER,
    COUNT, SUM, AVERAGE,
    BEGIN_TRANSACTION, COMMIT, ROLLBACK,
    BATCH_START, BATCH_EXECUTE, UNKNOWN
};

struct Command {
    CommandType type;
    Document document;
    std::string field;
    std::string value;

    Command() : type(CommandType::UNKNOWN) {}
};
