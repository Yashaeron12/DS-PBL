#include <ctype.h>
#include <string.h>
#include "expression_parser.h"
#include "stack.h"
#include "utils.h"


void infix_to_postfix(char* infix, char* postfix) {
Stack s;
initStack(&s);
int k = 0;


for (int i = 0; infix[i]; i++) {
char c = infix[i];
if (isdigit(c)) {
    while (isdigit(infix[i])) {
        postfix[k++] = infix[i++];
    }
    postfix[k++] = ' '; 
    i--; 
}

else if (c == '(') {
push(&s, c);
}
else if (c == ')') {
while (!isEmpty(&s) && peek(&s) != '(') {
postfix[k++] = pop(&s);
}
pop(&s);
}
else if (isOperator(c)) {
while (!isEmpty(&s) && precedence(peek(&s)) >= precedence(c)) {
postfix[k++] = pop(&s);
}
push(&s, c);
}
}
while (!isEmpty(&s)) {
postfix[k++] = pop(&s);
}
postfix[k] = '\0';
}