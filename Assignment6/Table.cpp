#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <sstream>

using namespace std;

// Grammar definition
vector<pair<string, string>> grammar = {
    {"E", "E+T"},
    {"E", "T"},
    {"T", "T*F"},
    {"T", "F"},
    {"F", "(E)"},
    {"F", "id"}
};

// Function to compute FIRST sets
map<string, set<string>> firstSet;
set<string> computeFirst(string symbol) {
    set<string> first;
    if (!isupper(symbol[0])) { // If terminal
        first.insert(symbol);
        return first;
    }
    for (auto& prod : grammar) {
        if (prod.first == symbol) {
            string body = prod.second;
            if (body != symbol) { // Avoid left recursion
                set<string> firstBody = computeFirst(string(1, body[0]));
                first.insert(firstBody.begin(), firstBody.end());
            }
        }
    }
    return first;
}

// Function to compute FOLLOW sets
map<string, set<string>> followSet;
void computeFollow() {
    followSet["E"].insert("$"); // Start symbol
    for (auto& prod : grammar) {
        string head = prod.first, body = prod.second;
        for (size_t i = 0; i < body.size(); i++) {
            if (isupper(body[i])) { // Non-terminal
                string B(1, body[i]);
                if (i + 1 < body.size()) { // Check next symbol
                    string next(1, body[i + 1]);
                    set<string> firstNext = computeFirst(next);
                    followSet[B].insert(firstNext.begin(), firstNext.end());
                    followSet[B].erase("ε");
                }
                if (i + 1 == body.size() || followSet[head].count("ε")) {
                    followSet[B].insert(followSet[head].begin(), followSet[head].end());
                }
            }
        }
    }
}

// LR(0) Item definition
struct Item {
    string head;
    string body;
    int dot; // Position of the dot

    bool operator<(const Item& other) const {
        return tie(head, body, dot) < tie(other.head, other.body, other.dot);
    }
};

// Function to compute Closure
set<Item> closure(set<Item> items) {
    queue<Item> workQueue;
    for (auto& item : items) workQueue.push(item);

    while (!workQueue.empty()) {
        Item current = workQueue.front();
        workQueue.pop();

        if (current.dot < current.body.size()) {
            char nextSymbol = current.body[current.dot];
            if (isupper(nextSymbol)) { // Non-terminal
                for (auto& prod : grammar) {
                    if (prod.first[0] == nextSymbol) {
                        Item newItem = { prod.first, prod.second, 0 };
                        if (items.insert(newItem).second) {
                            workQueue.push(newItem);
                        }
                    }
                }
            }
        }
    }

    return items;
}

// Function to compute Goto
set<Item> goTo(set<Item> items, char symbol) {
    set<Item> nextItems;
    for (auto& item : items) {
        if (item.dot < item.body.size() && item.body[item.dot] == symbol) {
            nextItems.insert({ item.head, item.body, item.dot + 1 });
        }
    }
    return closure(nextItems);
}

// Function to construct Canonical Collection
vector<set<Item>> canonicalSet() {
    vector<set<Item>> states;
    map<set<Item>, int> stateIndices;

    // Start state
    set<Item> startState = closure({ {"E'", "E", 0} });
    states.push_back(startState);
    stateIndices[startState] = 0;

    queue<set<Item>> workQueue;
    workQueue.push(startState);

    while (!workQueue.empty()) {
        set<Item> currentState = workQueue.front();
        workQueue.pop();

        set<char> symbols;
        for (auto& item : currentState) {
            if (item.dot < item.body.size()) {
                symbols.insert(item.body[item.dot]);
            }
        }

        for (char symbol : symbols) {
            set<Item> nextState = goTo(currentState, symbol);
            if (stateIndices.find(nextState) == stateIndices.end()) {
                stateIndices[nextState] = states.size();
                states.push_back(nextState);
                workQueue.push(nextState);
            }
        }
    }

    return states;
}

// Function to construct SLR Parsing Tables
void constructSLRTables(vector<set<Item>>& states) {
    // Action and Goto tables
    vector<map<string, string>> action(states.size());
    vector<map<string, int>> gotoTable(states.size());

    // Initialize Action and Goto Tables
    for (size_t i = 0; i < states.size(); i++) {
        for (auto& item : states[i]) {
            if (item.dot == item.body.size()) { // Reduce or Accept
                if (item.head == "E'" && item.body == "E") {
                    action[i]["$"] = "acc"; // Accept
                }
                else {
                    string production = item.head + "->" + item.body;
                    for (auto& follow : followSet[item.head]) {
                        string followSymbol = follow;
                        action[i][followSymbol] = "r " + production;
                    }
                }
            }
            else { // Shift or Goto
                char nextSymbol = item.body[item.dot];
                if (isupper(nextSymbol)) { // Non-terminal
                    gotoTable[i][string(1, nextSymbol)] = i + 1;
                }
                else { // Terminal
                    action[i][string(1, nextSymbol)] = "s" + to_string(i + 1);
                }
            }
        }
    }

    // Print Action Table
    cout << "\nAction Table:\n";
    for (size_t i = 0; i < action.size(); i++) {
        cout << "State " << i << ": ";
        for (auto& entry : action[i]) {
            cout << entry.first << " -> " << entry.second << ", ";
        }
        cout << "\n";
    }

    // Print Goto Table
    cout << "\nGoto Table:\n";
    for (size_t i = 0; i < gotoTable.size(); i++) {
        cout << "State " << i << ": ";
        for (auto& entry : gotoTable[i]) {
            cout << entry.first << " -> " << entry.second << ", ";
        }
        cout << "\n";
    }
}

int main() {
    // Compute FIRST and FOLLOW sets
    for (auto& prod : grammar) {
        firstSet[prod.first] = computeFirst(prod.first);
    }
    computeFollow();
    vector<set<Item>> states = canonicalSet();
    constructSLRTables(states);

    return 0;
}