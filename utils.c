#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Defined mathematical constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        case '!':
            return 4;
        default:
            return 0;
    }
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == '!';
}

int is_function_name(char* token) {
    return has_function(&function_ht, token);
}

int is_constant(char* token) {
    return strcmp(token, "pi") == 0 || strcmp(token, "e") == 0 || strcmp(token, "phi") == 0;
}

double get_constant(char* name) {
    if (strcmp(name, "pi") == 0) return M_PI;
    if (strcmp(name, "e") == 0) return M_E;
    if (strcmp(name, "phi") == 0) return (1.0 + sqrt(5.0)) / 2.0;
    
    printf("Unknown constant: %s\n", name);
    return 0;
}

double factorial(double n) {
    if (n < 0) return 0;
    if (n <= 1) return 1;
    if (n != (int)n) return 0;
    
    double result = 1;
    for (int i = 2; i <= (int)n; i++) {
        result *= i;
    }
    return result;
}

// Integration functions that bridge to the new data structures
double evaluate_function_integrated(char* func_name, double arg) {
    return call_function(&function_ht, func_name, arg);
}

double get_variable_integrated(char* name) {
    return get_variable_bst(&variable_bst, name);
}

int has_variable_integrated(char* name) {
    return has_variable_bst(&variable_bst, name);
}
