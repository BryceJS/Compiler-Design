#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

string readFile(string filename) {
    ifstream file(filename);
    string content;

    if (file.is_open()) {
        getline(file, content, '\0');  // Reads the entire file into the string 'content'
        file.close();
        return content;
    }
    else {
        return "";  // Return empty string if file can't be opened
    }
}

void outputWordsAndPunctuation(const string& content) {
    string word = "";

    for (char ch : content) {
        if (isalnum(ch)) {
            // Build words from alphanumeric characters
            word += ch;
        }
        else {
            // Output the word if any
            if (!word.empty()) {
                cout << word << endl;
                word.clear();
            }
            // Output punctuation separately
            if (ispunct(ch)) {
                cout << ch << endl;
            }
        }
    }

    // Output any remaining word at the end of the file
    if (!word.empty()) {
        cout << word << endl;
    }
}

int main() {
    string filename = "Text.txt";
    string content = readFile(filename);

    if (content.empty()) {
        cerr << "Error: Unable to read file or file is empty." << endl;
        return 1;  // Return 1 on error
    }

    outputWordsAndPunctuation(content);

    return 0;
}