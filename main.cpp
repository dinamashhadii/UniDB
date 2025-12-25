#include <iostream>
#include <string>
#include "engine/ExecutionEngine.h"
#include "parser/QueryParser.h"

using namespace std;

int main() {
    cout << "\n========================================" << endl;
    cout << "           UniDB         " << endl;
    cout << "========================================" << endl;
    ExecutionEngine engine;

    string input;
    while (true) {
        cout << "\nunidb> ";
        getline(cin, input);

        if (input == "exit" || input == "quit") {
            cout << "Goodbye!" << endl;
            break;
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