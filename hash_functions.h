// hash_functions.h - Hash Table for Function Storage
#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#define HASH_TABLE_SIZE 64

typedef struct FuncEntry {
    char* name;
    double (*func_ptr)(double);
    struct FuncEntry* next;  
} FuncEntry;

typedef struct {
    FuncEntry* table[HASH_TABLE_SIZE];
    int count;
} FunctionHashTable;

// Hash table operations
void init_function_hash_table(FunctionHashTable* ht);
void cleanup_function_hash_table(FunctionHashTable* ht);
unsigned int hash_function(const char* name);
void register_function(FunctionHashTable* ht, const char* name, double (*func_ptr)(double));
double call_function(FunctionHashTable* ht, const char* name, double arg);
int has_function(FunctionHashTable* ht, const char* name);
void list_functions(FunctionHashTable* ht);

// Built-in mathematical functions
double math_sin(double x);
double math_cos(double x);
double math_tan(double x);
double math_sqrt(double x);
double math_log(double x);
double math_ln(double x);
double math_abs(double x);
double math_ceil(double x);
double math_floor(double x);
double math_exp(double x);

void register_builtin_functions(FunctionHashTable* ht);

#endif
