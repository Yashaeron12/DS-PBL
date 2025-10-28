#include "stack.h"

void initStack(Stack* s) {
    s->top = -1;
}

void push(Stack* s, char c) {
    if (s->top < 99) {  // Add bounds check
        s->data[++s->top] = c;
    }
}

char pop(Stack* s) {
    if (s->top >= 0) {  // Add bounds check
        return s->data[s->top--];
    }
    return '\0';
}

char peek(Stack* s) {
    if (s->top >= 0) {  // Add bounds check
        return s->data[s->top];
    }
    return '\0';
}

int isEmpty(Stack* s) {
    return s->top == -1;
}
