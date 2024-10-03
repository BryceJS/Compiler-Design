% {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    enum TokenType {
        KEYWORD = 1,
        IDENTIFIER,
        OPERATOR,
        DELIMITER,
        INTNUM,
        REALNUM,
        STRING
    };

    const char* token_names[] = {
        "KEYWORD", "IDENTIFIER", "OPERATOR", "DELIMITER", "INTNUM", "REALNUM", "STRING"
    };

    %}

% option noyywrap

%%

"//".*                  { /* skip comments */ }
"int" | "float" | "if" | "else" | "while" | "for" | "return" | "break" | "continue" { return KEYWORD; }
[a - zA - Z][a - zA - Z0 - 9]{ 0,63 } { yylval.sval = strdup(yytext); return IDENTIFIER; }
[=<>!] = ? { return OPERATOR; }
[\(\)\{\}; ]             { return DELIMITER; }
[0 - 9] + { yylval.ival = atoi(yytext); return INTNUM; }
[0 - 9] + "."[0 - 9] * { yylval.fval = atof(yytext); return REALNUM; }
[0 - 9] + ("."[0 - 9] *) ? ([eE][+-] ? [0 - 9] + ) ? { yylval.fval = atof(yytext); return REALNUM; }
\"([^\\\"]|\\.)*\"      { yylval.sval = strdup(yytext); return STRING; }
[\t\n] + { /* ignore whitespace */ }
.                       { fprintf(stderr, "Unrecognized character: %s\n", yytext); }

%%

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Could not open %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }

    int token;
    while ((token = yylex()) != 0) {
        switch (token) {
        case IDENTIFIER:
        case STRING:
            printf("Token: %s, Value: %s\n", token_names[token - 1], yylval.sval);
            free(yylval.sval);  // Free allocated memory
            break;
        case INTNUM:
            printf("Token: %s, Value: %d\n", token_names[token - 1], yylval.ival);
            break;
        case REALNUM:
            printf("Token: %s, Value: %f\n", token_names[token - 1], yylval.fval);
            break;
        default:
            printf("Token: %s, Text: %s\n", token_names[token - 1], yytext);
        }
    }

    if (argc > 1) {
        fclose(yyin);
    }

    return 0;
}