#include <iostream>
#include <string>
#include "engine/ExecutionEngine.h"
#include "parser/QueryParser.h"

using namespace std;

void printHelp() {
    cout << "\n========================================" << endl;
    cout << "           UniDB NoSQL Database" << endl;
    cout << "========================================" << endl;
    cout << "\n📝 INSERT:" << endl;
    cout << "  db.students.insertOne({_id: 1, name: \"Ali\", gpa: 18.5})" << endl;

    cout << "\n❌ DELETE:" << endl;
    cout << "  db.students.deleteOne({_id: 1})" << endl;
    cout << "  db.students.deleteOne(1)" << endl;

    cout << "\n🔍 SEARCH:" << endl;
    cout << "  db.students.findById(1)" << endl;
    cout << "  db.students.findAll()" << endl;
    cout << "  db.students.filter(field: \"name\", value: \"Ali\")" << endl;

    cout << "\n📊 AGGREGATION:" << endl;
    cout << "  db.students.count()" << endl;
    cout << "  db.students.sum(field: \"gpa\")" << endl;
    cout << "  db.students.average(field: \"gpa\")" << endl;

    cout << "\n💾 TRANSACTIONS:" << endl;
    cout << "  db.beginTransaction()" << endl;
    cout << "  db.commit()" << endl;
    cout << "  db.rollback()" << endl;

    cout << "\n⚡ BATCH PROCESSING:" << endl;
    cout << "  db.batch.start()" << endl;
    cout << "  db.batch.execute()" << endl;

    cout << "\n📋 OTHER:" << endl;
    cout << "  help - Show this help" << endl;
    cout << "  demo - Run demonstration" << endl;
    cout << "  exit - Quit program" << endl;

    cout << "\n========================================" << endl;
}

void runDemo(ExecutionEngine& engine) {
    cout << "\n=== UniDB Demonstration ===" << endl;

    cout << "\n1. Inserting sample data:" << endl;
    cout << engine.execute(QueryParser::parse("db.students.insertOne({_id: 101, name: \"Ali\", gpa: 18.5})")) << endl;
    cout << engine.execute(QueryParser::parse("db.students.insertOne({_id: 102, name: \"Sara\", gpa: 19.2})")) << endl;
    cout << engine.execute(QueryParser::parse("db.students.insertOne({_id: 103, name: \"Reza\", gpa: 17.8})")) << endl;

    cout << "\n2. Finding all documents:" << endl;
    cout << engine.execute(QueryParser::parse("db.students.findAll()")) << endl;

    cout << "\n3. Transaction demonstration:" << endl;
    cout << engine.execute(QueryParser::parse("db.beginTransaction()")) << endl;
    cout << engine.execute(QueryParser::parse("db.students.insertOne({_id: 104, name: \"Mina\", gpa: 20.0})")) << endl;
    cout << "Count before rollback: " << engine.execute(QueryParser::parse("db.students.count()")) << endl;
    cout << engine.execute(QueryParser::parse("db.rollback()")) << endl;
    cout << "Count after rollback: " << engine.execute(QueryParser::parse("db.students.count()")) << endl;

    cout << "\n4. Aggregation functions:" << endl;
    cout << engine.execute(QueryParser::parse("db.students.sum(field: \"gpa\")")) << endl;
    cout << engine.execute(QueryParser::parse("db.students.average(field: \"gpa\")")) << endl;

    cout << "\n5. Batch processing:" << endl;
    cout << engine.execute(QueryParser::parse("db.batch.start()")) << endl;
    cout << engine.execute(QueryParser::parse("db.students.deleteOne({_id: 101})")) << endl;
    cout << engine.execute(QueryParser::parse("db.students.insertOne({_id: 105, name: \"Amir\", gpa: 16.5})")) << endl;
    cout << engine.execute(QueryParser::parse("db.batch.execute()")) << endl;

    cout << "\n6. Final state:" << endl;
    cout << engine.execute(QueryParser::parse("db.students.findAll()")) << endl;

    cout << "\n=== Demonstration Complete ===" << endl;
}

int main() {
    cout << "=== UniDB NoSQL Database ===" << endl;
    cout << "Type 'help' for commands, 'demo' for demonstration, 'exit' to quit" << endl;

    ExecutionEngine engine;

    string input;
    while (true) {
        cout << "\nunidb> ";
        getline(cin, input);

        if (input == "exit" || input == "quit") {
            cout << "Goodbye!" << endl;
            break;
        }

        if (input == "help") {
            printHelp();
            continue;
        }

        if (input == "demo") {
            runDemo(engine);
            continue;
        }

        if (input.empty()) {
            continue;
        }

        try {
            Command cmd = QueryParser::parse(input);
            string result = engine.execute(cmd);
            cout << result << endl;
        } catch (const exception& e) {
            cout << "ERROR: " << e.what() << endl;
        }
    }

    return 0;
}