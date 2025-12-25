#include <iostream>
#include <string>
#include "src/engine/ExecutionEngine.h"
#include "src/parser/QueryParser.h"

using namespace std;

void printHelp() {
    cout << "\n=== UniDB NoSQL Database ===" << endl;
    cout << "Commands:" << endl;
    cout << "  db.students.insertOne({_id: 1, name: \"Ali\", gpa: 18.5})" << endl;
    cout << "  db.students.deleteOne({_id: 1})" << endl;
    cout << "  db.students.findById(1)" << endl;
    cout << "  db.students.findAll()" << endl;
    cout << "  db.students.filter(field: \"name\", value: \"Ali\")" << endl;
    cout << "  db.students.count()" << endl;
    cout << "  db.students.sum(field: \"gpa\")" << endl;
    cout << "  db.students.average(field: \"gpa\")" << endl;
    cout << "  db.beginTransaction()" << endl;
    cout << "  db.commit()" << endl;
    cout << "  db.rollback()" << endl;
    cout << "  db.batch.start()" << endl;
    cout << "  db.batch.execute()" << endl;
    cout << "\nType 'exit' to quit, 'help' for commands" << endl;
}

int main() {
    cout << "=== UniDB NoSQL Database ===" << endl;
    cout << "Type 'help' for commands, 'exit' to quit" << endl;

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