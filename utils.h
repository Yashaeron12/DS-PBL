#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include "bst_variables.h"
#include "hash_functions.h"

// External references to global data structures
extern VariableBST variable_bst;
extern FunctionHashTable function_ht;

// Mathematical constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

// Operator and parsing functions
int precedence(char op);
int is_operator(char c);
int is_function_name(char* token);
int is_constant(char* token);

// Mathematical operations
double get_constant(char* name);
double factorial(double n);

// Integration functions
double evaluate_function_integrated(char* func_name, double arg);
double get_variable_integrated(char* name);
int has_variable_integrated(char* name);

#endif
