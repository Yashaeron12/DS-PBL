#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "evaluator.h"
#include "utils.h"

typedef struct {
    double data[100];
    int top;
} DoubleStack;

void initDoubleStack(DoubleStack* s) {
    s->top = -1;
}

void pushDouble(DoubleStack* s, double val) {
    if (s->top < 99) {
        s->data[++s->top] = val;
    }
}

double popDouble(DoubleStack* s) {
    if (s->top >= 0) {
        return s->data[s->top--];
    }
    return 0;
}

double evaluate_postfix(char* postfix) {
    DoubleStack s;
    initDoubleStack(&s);
    
    char postfix_copy[512];
    strcpy(postfix_copy, postfix);
    
    char* token = strtok(postfix_copy, " ");
    
    while (token != NULL) {
        // Handle numbers
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1]))) {
            pushDouble(&s, atof(token));
        }
        // Handle functions - Change: use integrated function
        else if (strncmp(token, "func_", 5) == 0) {
            char* func_name = token + 5;
            double arg = popDouble(&s);
            double result = evaluate_function_integrated(func_name, arg);
            pushDouble(&s, result);
        }
        // Handle constants
        else if (strncmp(token, "const_", 6) == 0) {
            char* const_name = token + 6;
            double value = get_constant(const_name);
            pushDouble(&s, value);
        }
        // Handle variables - CHANGE: use integrated function
        else if (strncmp(token, "var_", 4) == 0) {
            char* var_name = token + 4;
            double value = get_variable_integrated(var_name);
            pushDouble(&s, value);
        }
        // Handle function marker
        else if (token[0] == 'f' && strlen(token) == 1) {
            // Function already processed above
        }
        // Handle factorial
        else if (token[0] == '!' && strlen(token) == 1) {
            double a = popDouble(&s);
            double result = factorial(a);
            pushDouble(&s, result);
        }
        // Handle binary operators
        else if (strlen(token) == 1 && is_operator(token[0]) && token[0] != '!') {
            double b = popDouble(&s);
            double a = popDouble(&s);
            double result = 0;
            
            switch (token[0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': 
                    if (b != 0) result = a / b;
                    else {
                        printf("Error: Division by zero\n");
                        return 0;
                    }
                    break;
                case '^': result = pow(a, b); break;
                case '%': 
                    if (b != 0) result = fmod(a, b);
                    else {
                        printf("Error: Modulo by zero\n");
                        return 0;
                    }
                    break;
                default: 
                    printf("Unknown operator: %c\n", token[0]);
                    return 0;
            }
            
            pushDouble(&s, result);
        }
        
        token = strtok(NULL, " ");
    }
    
    return popDouble(&s);
}
