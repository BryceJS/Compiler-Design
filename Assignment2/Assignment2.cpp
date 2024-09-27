% {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
    %}

% option noyywrap

%%

"//".*                  {/* skip comments */}
"int" | "real" | "if" | "then" | "else" | "while"   { return KEYWORD; }
[a - zA - Z][a - zA - Z0 - 9]{ 0,63 } { yylval.sval = strdup(yytext); return IDENTIFIER; }
[=<>!] = ? { return OPERATOR; }
[\(\)\{\}; ]             { return DELIMITER; }
[0 - 9] + { yylval.ival = atoi(yytext); return INTNUM; }
[0 - 9] + "."[0 - 9] * ([eE][+-] ? [0 - 9] + ) ? { yylval.fval = atof(yytext); return REALNUM; }
%%

int main(int argc, char** argv) {
    int token;
    while ((token = yylex()) != 0) {
        printf("Token: %d, Value: %s\n", token, yytext);
    }
    return 0;
}