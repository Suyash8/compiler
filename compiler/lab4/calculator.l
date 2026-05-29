%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>

int result = 0;
int current_number = 0;
char operator = '+';   // default operator
%}

DIGIT   [0-9]

%%

{DIGIT}+  {
              current_number = atoi(yytext);

              if(operator == '+')
                  result += current_number;
              else if(operator == '-')
                  result -= current_number;
          }

"+"       { operator = '+'; }

"-"       { operator = '-'; }

\n        { 
              printf("Result = %d\n", result);
              return 0;
          }

[ \t\n]   ;   /* Ignore whitespace */

.         { printf("Invalid character: %s\n", yytext); }

%%

int main() {
    printf("Enter expression: ");
    yylex();
    return 0;
}