#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>

using namespace std;

// Function to check if a character is a punctuation mark
bool isPunctuation(char c) {
    return ispunct(c) && !isspace(c);
}

// Function to process the file and display words and punctuation marks
void processFile(const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1); // Exit with error code 1
    }

    string line;
    while (getline(file, line)) {
        istringstream lineStream(line);
        string word;

        // Extract words and punctuation marks from the line
        while (lineStream >> word) {
            for (char& c : word) {
                // Print each character if it's punctuation
                if (isPunctuation(c)) {
                    cout << c << endl;
                }
            }
            // Print the word itself
            cout << word << endl;
        }

        // Handle punctuation marks directly in the line
        for (char c : line) {
            if (isPunctuation(c)) {
                cout << c << endl;
            }
        }
    }

    file.close();
}

int main() {
    string filename = "Text.txt"; // Use double backslashes for Windows paths

    processFile(filename);

    return 0;
}