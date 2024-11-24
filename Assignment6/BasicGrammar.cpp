#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <queue>
#include <iomanip>

using namespace std;

// Grammar Rule Structure
struct Rule {
    char lhs;                   // Left-hand side (Non-terminal)
    string rhs;                 // Right-hand side (Production)
};

// Grammar Rules (You can modify these for Advanced Grammar)
vector<Rule> grammar = {
    {'E', "E+T"},
    {'E', "T"},
    {'T', "T*F"},
    {'T', "F"},
    {'F', "(E)"},
    {'F', "id"}
};

// First and Follow Sets
map<char, set<char>> firstSet, followSet;

// Canonical Collection and Parsing Table
vector<map<string, set<pair<int, char>>>> canonicalCollection;
map<int, map<char, string>> actionTable, gotoTable;

// Function to Compute Closure
set<pair<int, char>> closure(set<pair<int, char>> items) {
    queue<pair<int, char>> toProcess;
    for (auto item : items) toProcess.push(item);

    while (!toProcess.empty()) {
        auto item = toProcess.front();
        toProcess.pop();
        int ruleIdx = item.first;
        char lookahead = item.second;

        string rhs = grammar[ruleIdx].rhs;
        size_t dotPos = rhs.find('.');
        if (dotPos != string::npos && dotPos + 1 < rhs.size()) {
            char nextSymbol = rhs[dotPos + 1];
            if (isupper(nextSymbol)) {
                for (size_t i = 0; i < grammar.size(); ++i) {
                    if (grammar[i].lhs == nextSymbol) {
                        auto newItem = make_pair(i, lookahead);
                        if (items.insert(newItem).second) {
                            toProcess.push(newItem);
                        }
                    }
                }
            }
        }
    }
    return items;
}

// Function to Compute Goto
set<pair<int, char>> goTo(set<pair<int, char>> state, char symbol) {
    set<pair<int, char>> newState;
    for (auto item : state) {
        int ruleIdx = item.first;
        char lookahead = item.second;
        string rhs = grammar[ruleIdx].rhs;

        size_t dotPos = rhs.find('.');
        if (dotPos != string::npos && dotPos + 1 < rhs.size() && rhs[dotPos + 1] == symbol) {
            string newRhs = rhs.substr(0, dotPos) + symbol + "." + rhs.substr(dotPos + 2);
            newState.insert({ ruleIdx, lookahead });
        }
    }
    return closure(newState);
}

// Function to Compute Canonical Collection
void computeCanonicalCollection() {
    // Start State
    set<pair<int, char>> startState = closure({ {0, '$'} });
    canonicalCollection.push_back({ {"", startState} });
    queue<set<pair<int, char>>> toProcess;
    toProcess.push(startState);

    while (!toProcess.empty()) {
        auto state = toProcess.front();
        toProcess.pop();

        for (char symbol : "E+T*F(id)$") {
            auto newState = goTo(state, symbol);
            if (!newState.empty() && find(canonicalCollection.begin(), canonicalCollection.end(), newState) == canonicalCollection.end()) {
                canonicalCollection.push_back({ {"", newState} });
                toProcess.push(newState);
            }
        }
    }
}

// Function to Build SLR Parse Table
void buildParseTable() {
    // Implementation to compute parsing actions and goto transitions
}

// Display Action and Goto Table
void displayParseTable() {
    // Implementation to display the parsing table
}

int main() {
    computeCanonicalCollection();
    buildParseTable();
    displayParseTable();

    return 0;
}