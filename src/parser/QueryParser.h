#ifndef QUERYPARSER_H
#define QUERYPARSER_H

#include "../engine/Command.h"
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

class QueryParser {
public:
    static Command parse(const std::string& query) {
        std::string trimmedQuery = query;
        trimmedQuery.erase(0, trimmedQuery.find_first_not_of(" \t\n\r"));
        trimmedQuery.erase(trimmedQuery.find_last_not_of(" \t\n\r") + 1);

        if (trimmedQuery.find("db.students.insertOne") == 0) {
            return parseInsertOne(trimmedQuery);
        }
        else if (trimmedQuery.find("db.students.deleteOne") == 0) {
            return parseDeleteOne(trimmedQuery);
        }
        else if (trimmedQuery.find("db.students.findById") == 0) {
            return parseFindById(trimmedQuery);
        }
        else if (trimmedQuery.find("db.students.findAll") == 0) {
            Command cmd;
            cmd.type = CommandType::FIND_ALL;
            return cmd;
        }
        else if (trimmedQuery.find("db.students.filter") == 0) {
            return parseFilter(trimmedQuery);
        }
        else if (trimmedQuery.find("db.students.count") == 0) {
            Command cmd;
            cmd.type = CommandType::COUNT;
            return cmd;
        }
        else if (trimmedQuery.find("db.students.sum") == 0) {
            Command cmd;
            cmd.type = CommandType::SUM;
            cmd.field = "gpa";
            return cmd;
        }
        else if (trimmedQuery.find("db.students.average") == 0) {
            Command cmd;
            cmd.type = CommandType::AVERAGE;
            cmd.field = "gpa";
            return cmd;
        }
        else if (trimmedQuery.find("db.beginTransaction") == 0) {
            Command cmd;
            cmd.type = CommandType::BEGIN_TRANSACTION;
            return cmd;
        }
        else if (trimmedQuery.find("db.commit") == 0) {
            Command cmd;
            cmd.type = CommandType::COMMIT;
            return cmd;
        }
        else if (trimmedQuery.find("db.rollback") == 0) {
            Command cmd;
            cmd.type = CommandType::ROLLBACK;
            return cmd;
        }
        else if (trimmedQuery.find("db.batch.start") == 0) {
            Command cmd;
            cmd.type = CommandType::BATCH_START;
            return cmd;
        }
        else if (trimmedQuery.find("db.batch.execute") == 0) {
            Command cmd;
            cmd.type = CommandType::BATCH_EXECUTE;
            return cmd;
        }

        Command cmd;
        cmd.type = CommandType::UNKNOWN;
        return cmd;
    }

private:
    static Command parseInsertOne(const std::string& query) {
        Command cmd;
        cmd.type = CommandType::INSERT_ONE;

        size_t start = query.find('(');
        size_t end = query.find(')');
        if (start == std::string::npos || end == std::string::npos) {
            return cmd;
        }

        std::string content = query.substr(start + 1, end - start - 1);

        size_t idPos = content.find("_id:");
        if (idPos != std::string::npos) {
            size_t comma = content.find(',', idPos);
            if (comma == std::string::npos) comma = content.find('}', idPos);
            std::string idStr = content.substr(idPos + 4, comma - idPos - 4);
            idStr.erase(0, idStr.find_first_not_of(" \t"));
            idStr.erase(idStr.find_last_not_of(" \t") + 1);
            cmd.document.id = std::stoi(idStr);
        }

        size_t namePos = content.find("name:");
        if (namePos != std::string::npos) {
            size_t quote1 = content.find('"', namePos);
            size_t quote2 = content.find('"', quote1 + 1);
            if (quote1 != std::string::npos && quote2 != std::string::npos) {
                cmd.document.name = content.substr(quote1 + 1, quote2 - quote1 - 1);
            }
        }

        size_t gpaPos = content.find("gpa:");
        if (gpaPos != std::string::npos) {
            size_t comma = content.find(',', gpaPos);
            if (comma == std::string::npos) comma = content.find('}', gpaPos);
            std::string gpaStr = content.substr(gpaPos + 4, comma - gpaPos - 4);
            gpaStr.erase(0, gpaStr.find_first_not_of(" \t"));
            gpaStr.erase(gpaStr.find_last_not_of(" \t") + 1);
            cmd.document.gpa = std::stod(gpaStr);
        }

        return cmd;
    }

    static Command parseDeleteOne(const std::string& query) {
        Command cmd;
        cmd.type = CommandType::DELETE_ONE;

        size_t start = query.find('(');
        size_t end = query.find(')');
        if (start == std::string::npos || end == std::string::npos) {
            return cmd;
        }

        std::string content = query.substr(start + 1, end - start - 1);

        size_t idPos = content.find("_id:");
        if (idPos != std::string::npos) {
            size_t endPos = content.find('}', idPos);
            std::string idStr = content.substr(idPos + 4, endPos - idPos - 4);
            idStr.erase(0, idStr.find_first_not_of(" \t"));
            idStr.erase(idStr.find_last_not_of(" \t") + 1);
            cmd.document.id = std::stoi(idStr);
        } else {
            content.erase(0, content.find_first_not_of(" \t"));
            content.erase(content.find_last_not_of(" \t") + 1);

            if (content.front() == '{') content = content.substr(1);
            if (content.back() == '}') content.pop_back();

            content.erase(0, content.find_first_not_of(" \t"));
            content.erase(content.find_last_not_of(" \t") + 1);

            std::string idStr;
            for (char c : content) {
                if (std::isdigit(c)) idStr += c;
            }

            if (!idStr.empty()) {
                cmd.document.id = std::stoi(idStr);
            }
        }

        return cmd;
    }

    static Command parseFindById(const std::string& query) {
        Command cmd;
        cmd.type = CommandType::FIND_BY_ID;

        size_t start = query.find('(');
        size_t end = query.find(')');
        if (start == std::string::npos || end == std::string::npos) {
            return cmd;
        }

        std::string idStr = query.substr(start + 1, end - start - 1);
        idStr.erase(0, idStr.find_first_not_of(" \t"));
        idStr.erase(idStr.find_last_not_of(" \t") + 1);
        cmd.document.id = std::stoi(idStr);

        return cmd;
    }

    static Command parseFilter(const std::string& query) {
        Command cmd;
        cmd.type = CommandType::FILTER;

        size_t start = query.find('(');
        size_t end = query.find(')');
        if (start == std::string::npos || end == std::string::npos) {
            return cmd;
        }

        std::string content = query.substr(start + 1, end - start - 1);

        size_t fieldPos = content.find("field:");
        if (fieldPos != std::string::npos) {
            size_t quote1 = content.find('"', fieldPos);
            size_t quote2 = content.find('"', quote1 + 1);
            if (quote1 != std::string::npos && quote2 != std::string::npos) {
                cmd.field = content.substr(quote1 + 1, quote2 - quote1 - 1);
            }
        }
        size_t valuePos = content.find("value:");
        if (valuePos != std::string::npos) {
            size_t quote1 = content.find('"', valuePos);
            size_t quote2 = content.find('"', quote1 + 1);
            if (quote1 != std::string::npos && quote2 != std::string::npos) {
                cmd.value = content.substr(quote1 + 1, quote2 - quote1 - 1);
            }
        }
        return cmd;
    }
};

#endif