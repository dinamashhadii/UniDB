#pragma once
#include <string>
#include "engine/Command.h"

class QueryParser {
public:
    static Command parse(const std::string& query);

private:
    static Command parseInsert(const std::string& query);
    static Command parseDelete(const std::string& query);
    static Command parseFindById(const std::string& query);
    static Command parseFilter(const std::string& query);
};
