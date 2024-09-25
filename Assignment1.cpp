"//".*                                                         /*comment line - actions: skip */
"int"|"real"|"if"|"then"|"else"|"while"                        {return KEYWORD; }
[a - zA - Z][a - Za - Z0 - 9]{ 0,63 }                          {yylval.sval = strdup(yytext); trturn IDENTIFIER; }
[=-/= <>!] +                                                   {return OPERATOR; }
[() {}; ]                                                      {return DELIMITER; }
[0 - 9] +                                                                             { yylval = atoi(yytext); return INT; }
[eE][+| -] ? [0 - 9] +                                                                { yylval = atoi(yytext); return REAL; }
\.[0-9]+                                                                              { yylval = atoi(yytext); return REAL; }
[0 - 9] + ([eE][+| -] ? [0 - 9] + ).[0 - 9] + ([eE][+| -] ? [0 - 9] + | ε)            { yyfval = atof(yytext); return REAL; }