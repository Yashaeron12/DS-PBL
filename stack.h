#ifndef STACK_H
#define STACK_H


typedef struct {
char items[256];
int top;
} Stack;


void initStack(Stack* s);
int isEmpty(Stack* s);
void push(Stack* s, char c);
char pop(Stack* s);
char peek(Stack* s);


#endif