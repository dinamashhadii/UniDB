#include "parser/QueryParser.h"
#include <sstream>

static size_t findParenOpen(const std::string& s) { return s.find('('); }
static size_t findParenClose(const std::string& s) { return s.find(')'); }

Command QueryParser::parse(const std::string& query) {
    if (query.find("db.students.insertOne") == 0) return parseInsert(query);
    else if (query.find("db.students.deleteOne") == 0) return parseDelete(query);
    else if (query.find("db.students.findById") == 0) return parseFindById(query);
    else if (query.find("db.students.findAll") == 0) { Command c; c.type = CommandType::FIND_ALL; return c; }
    else if (query.find("db.students.filter") == 0) return parseFilter(query);
    else if (query.find("db.students.count") == 0) { Command c; c.type = CommandType::COUNT; return c; }
    else if (query.find("db.students.sum") == 0) { Command c; c.type = CommandType::SUM; c.field = "gpa"; return c; }
    else if (query.find("db.students.average") == 0) { Command c; c.type = CommandType::AVERAGE; c.field = "gpa"; return c; }
    else if (query.find("db.beginTransaction") == 0) { Command c; c.type = CommandType::BEGIN_TRANSACTION; return c; }
    else if (query.find("db.commit") == 0) { Command c; c.type = CommandType::COMMIT; return c; }
    else if (query.find("db.rollback") == 0) { Command c; c.type = CommandType::ROLLBACK; return c; }
    else if (query.find("db.batch.start") == 0) { Command c; c.type = CommandType::BATCH_START; return c; }
    else if (query.find("db.batch.execute") == 0) { Command c; c.type = CommandType::BATCH_EXECUTE; return c; }

    Command c; c.type = CommandType::UNKNOWN; return c;
}

Command QueryParser::parseInsert(const std::string& query) {
    Command cmd; cmd.type = CommandType::INSERT_ONE;

    size_t start = findParenOpen(query);
    size_t end = findParenClose(query);
    if (start == std::string::npos || end == std::string::npos) return cmd;

    std::string content = query.substr(start + 1, end - start - 1);

    size_t idPos = content.find("_id:");
    if (idPos != std::string::npos) {
        size_t comma = content.find(',', idPos);
        if (comma == std::string::npos) comma = content.find('}', idPos);
        std::string idStr = content.substr(idPos + 4, comma - idPos - 4);
        cmd.document.id = std::stoi(idStr);
    }

    size_t namePos = content.find("name:");
    if (namePos != std::string::npos) {
        size_t q1 = content.find('"', namePos);
        size_t q2 = content.find('"', q1 + 1);
        if (q1 != std::string::npos && q2 != std::string::npos) {
            cmd.document.name = content.substr(q1 + 1, q2 - q1 - 1);
        }
    }

    size_t gpaPos = content.find("gpa:");
    if (gpaPos != std::string::npos) {
        size_t comma = content.find(',', gpaPos);
        if (comma == std::string::npos) comma = content.find('}', gpaPos);
        std::string gpaStr = content.substr(gpaPos + 4, comma - gpaPos - 4);
        cmd.document.gpa = std::stod(gpaStr);
    }

    return cmd;
}

Command QueryParser::parseDelete(const std::string& query) {
    Command cmd; cmd.type = CommandType::DELETE_ONE;

    size_t start = findParenOpen(query);
    size_t end = findParenClose(query);
    if (start == std::string::npos || end == std::string::npos) return cmd;

    std::string content = query.substr(start + 1, end - start - 1);

    size_t idPos = content.find("_id:");
    if (idPos != std::string::npos) {
        size_t endPos = content.find('}', idPos);
        std::string idStr = content.substr(idPos + 4, endPos - idPos - 4);
        cmd.document.id = std::stoi(idStr);
    } else {
        std::string idStr;
        for (char c : content) if (std::isdigit(static_cast<unsigned char>(c))) idStr += c;
        if (!idStr.empty()) cmd.document.id = std::stoi(idStr);
    }

    return cmd;
}

Command QueryParser::parseFindById(const std::string& query) {
    Command cmd; cmd.type = CommandType::FIND_BY_ID;

    size_t start = findParenOpen(query);
    size_t end = findParenClose(query);
    if (start == std::string::npos || end == std::string::npos) return cmd;

    std::string idStr = query.substr(start + 1, end - start - 1);
    cmd.document.id = std::stoi(idStr);

    return cmd;
}

Command QueryParser::parseFilter(const std::string& query) {
    Command cmd; cmd.type = CommandType::FILTER;

    size_t start = findParenOpen(query);
    size_t end = findParenClose(query);
    if (start == std::string::npos || end == std::string::npos) return cmd;

    std::string content = query.substr(start + 1, end - start - 1);

    size_t fieldPos = content.find("field:");
    if (fieldPos != std::string::npos) {
        size_t q1 = content.find('"', fieldPos);
        size_t q2 = content.find('"', q1 + 1);
        if (q1 != std::string::npos && q2 != std::string::npos) {
            cmd.field = content.substr(q1 + 1, q2 - q1 - 1);
        }
    }

    size_t valuePos = content.find("value:");
    if (valuePos != std::string::npos) {
        size_t q1 = content.find('"', valuePos);
        size_t q2 = content.find('"', q1 + 1);
        if (q1 != std::string::npos && q2 != std::string::npos) {
            cmd.value = content.substr(q1 + 1, q2 - q1 - 1);
        }
    }

    return cmd;
}
