#include "utils.h"


int precedence(char op) {
if (op == '+' || op == '-') return 1;
if (op == '*' || op == '/') return 2;
if (op == '^') return 3;
return 0;
}


int isOperator(char c) {
return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}