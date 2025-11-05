#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "expression_parser.h"
#include "stack.h"
#include "utils.h"

// Simple function name storage for parsing
static char current_function[32] = {0};

void infix_to_postfix(char* infix, char* postfix) {
    Stack s;
    initStack(&s);
    
    int j = 0;
    int i = 0;
    
    while (infix[i]) {
        char c = infix[i];
        
        if (isspace(c)) {
            i++;
            continue;
        }
        
        // Handle numbers (including decimals)
        if (isdigit(c) || c == '.') {
            while (isdigit(infix[i]) || infix[i] == '.') {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            continue;
        }
        
        // Handle functions and variables
        if (isalpha(c)) {
            char token[32] = {0};
            int k = 0;
            
            while (isalnum(infix[i]) || infix[i] == '_') {
                token[k++] = infix[i++];
            }
            
            if (is_function_name(token)) {
                // Store function name for later use
                strcpy(current_function, token);
                push(&s, 'f');
            } else if (is_constant(token)) {
                sprintf(postfix + j, "const_%s ", token);
                j += strlen(postfix + j);
            } else {
                sprintf(postfix + j, "var_%s ", token);
                j += strlen(postfix + j);
            }
            continue;
        }
        
        // Handle operators and parentheses
        switch (c) {
            case '(':
                push(&s, c);
                break;
                
            case ')':
                while (!isEmpty(&s) && peek(&s) != '(') {
                    char op = pop(&s);
                    postfix[j++] = op;
                    postfix[j++] = ' ';
                }
                if (!isEmpty(&s)) {
                    char paren = pop(&s);
                }
                
                // Check if there's a function marker below the parentheses
                if (!isEmpty(&s) && peek(&s) == 'f') {
                    sprintf(postfix + j, "func_%s ", current_function);
                    j += strlen(postfix + j);
                    pop(&s); // Remove the function marker
                }
                break;
                
            case '!':
                postfix[j++] = c;
                postfix[j++] = ' ';
                break;
                
            default:
                if (is_operator(c)) {
                    while (!isEmpty(&s) && peek(&s) != '(' && peek(&s) != 'f' &&
                           precedence(peek(&s)) >= precedence(c)) {
                        postfix[j++] = pop(&s);
                        postfix[j++] = ' ';
                    }
                    push(&s, c);
                }
                break;
        }
        i++;
    }
    
    while (!isEmpty(&s)) {
        char op = pop(&s);
        if (op == 'f') {
            // Function markers should not be added to postfix at the end
            continue;
        } else {
            postfix[j++] = op;
            postfix[j++] = ' ';
        }
    }
    
    postfix[j] = '\0';
}