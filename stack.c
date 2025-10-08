#include "stack.h"


void initStack(Stack* s) {
s->top = -1;
}


int isEmpty(Stack* s) {
return s->top == -1;
}


void push(Stack* s, char c) {
s->items[++(s->top)] = c;
}


char pop(Stack* s) {
return s->items[(s->top)--];
}


char peek(Stack* s) {
return s->items[s->top];
}