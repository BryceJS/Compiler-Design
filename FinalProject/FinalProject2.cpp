#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stack>
using namespace std;

// Token structure to represent lexical units like numbers, operators, and parentheses
enum TokenType { ID, NUMBER, PLUS, MULTIPLY, LPAREN, RPAREN, END };

struct Token {
    TokenType type; // Type of the token (e.g., NUMBER, PLUS)
    string value;   // Actual value of the token (e.g., "4", "+")
};

// Lexical Analyzer (Lexer): Breaks input string into tokens
class Lexer {
public:
    Lexer(const string& input) : input(input), pos(0) {}

    // Returns the next token from the input string
    Token getNextToken() {
        // Skip whitespace
        while (pos < input.length() && isspace(input[pos])) pos++;

        // Check if we've reached the end of input
        if (pos >= input.length()) return { END, "" };

        char current = input[pos];

        // If the character is a digit, parse the entire number
        if (isdigit(current)) {
            string num;
            while (pos < input.length() && isdigit(input[pos])) {
                num += input[pos++];
            }
            return { NUMBER, num };
        }

        // Recognize individual operators and parentheses
        if (current == '+') { pos++; return { PLUS, "+" }; }
        if (current == '*') { pos++; return { MULTIPLY, "*" }; }
        if (current == '(') { pos++; return { LPAREN, "(" }; }
        if (current == ')') { pos++; return { RPAREN, ")" }; }

        // Handle unexpected characters
        cerr << "Unknown character: " << current << endl;
        exit(1);
    }

private:
    string input;  // The input string to tokenize
    size_t pos;    // Current position in the input
};

// Parser and Code Generator: Parses the tokens and generates intermediate code
class Parser {
public:
    Parser(Lexer& lexer) : lexer(lexer), currentToken(lexer.getNextToken()) {}

    // Main parsing function
    void parse() {
        expr(); // Parse the input according to the grammar
        if (currentToken.type != END) {
            cerr << "Unexpected token at the end" << endl;
            exit(1);
        }
    }

    // Prints the generated intermediate code
    void printIntermediateCode() {
        while (!intermediateCode.empty()) {
            cout << intermediateCode.back() << endl;
            intermediateCode.pop_back();
        }
    }

private:
    Lexer& lexer;                   // Reference to the lexer
    Token currentToken;             // Current token being processed
    vector<string> intermediateCode; // Stores the intermediate code
    int tempCounter = 0;            // Counter for temporary variables

    // Consumes the current token if it matches the expected type
    void consume(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        }
        else {
            cerr << "Unexpected token: " << currentToken.value << endl;
            exit(1);
        }
    }

    // Parses an expression (E -> E + T | T)
    void expr() {
        term(); // Parse the first term
        while (currentToken.type == PLUS) { // Handle '+' operators
            consume(PLUS);
            term(); // Parse the next term
            generateCode("+"); // Generate code for addition
        }
    }

    // Parses a term (T -> T * F | F)
    void term() {
        factor(); // Parse the first factor
        while (currentToken.type == MULTIPLY) { // Handle '*' operators
            consume(MULTIPLY);
            factor(); // Parse the next factor
            generateCode("*"); // Generate code for multiplication
        }
    }

    // Parses a factor (F -> (E) | id)
    void factor() {
        if (currentToken.type == NUMBER) {
            // If it's a number, load it into intermediate code
            intermediateCode.push_back("LOAD " + currentToken.value);
            consume(NUMBER);
        }
        else if (currentToken.type == LPAREN) {
            // If it's a '(', parse the enclosed expression
            consume(LPAREN);
            expr();
            consume(RPAREN);
        }
        else {
            cerr << "Unexpected token in factor: " << currentToken.value << endl;
            exit(1);
        }
    }

    // Generates intermediate code for an operation
    void generateCode(const string& op) {
        string rhs = "TEMP" + to_string(tempCounter++); // Right-hand side temporary variable
        intermediateCode.push_back("STORE " + rhs);

        string lhs = "TEMP" + to_string(tempCounter++); // Left-hand side temporary variable
        intermediateCode.push_back("STORE " + lhs);

        string result = "TEMP" + to_string(tempCounter++); // Result temporary variable
        intermediateCode.push_back("OPER " + op + " " + lhs + " " + rhs + " -> " + result);
    }
};

int main() {
    // Input expression to parse
    string input = "4+7*14+(7+6)*3";

    // Create a Lexer and a Parser
    Lexer lexer(input);
    Parser parser(lexer);

    cout << "Parsing the input..." << endl;
    parser.parse(); // Start the parsing process

    // Output the intermediate code
    cout << "Intermediate code:" << endl;
    parser.printIntermediateCode();

    return 0;
}