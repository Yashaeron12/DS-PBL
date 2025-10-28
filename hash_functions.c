// hash_functions.c - Hash Table Implementation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash_functions.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif
void init_function_hash_table(FunctionHashTable* ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    ht->count = 0;
    register_builtin_functions(ht);
}

unsigned int hash_function(const char* name) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *name++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    
    return hash % HASH_TABLE_SIZE;
}

void register_function(FunctionHashTable* ht, const char* name, double (*func_ptr)(double)) {
    unsigned int index = hash_function(name);
    
    // Check if function already exists
    FuncEntry* current = ht->table[index];
    while (current) {
        if (strcmp(current->name, name) == 0) {
            current->func_ptr = func_ptr;  // Update existing
            return;
        }
        current = current->next;
    }
    
    // Create new entry
    FuncEntry* new_entry = malloc(sizeof(FuncEntry));
    if (!new_entry) {
        printf("Error: Memory allocation failed!\n");
        return;
    }
    
    new_entry->name = malloc(strlen(name) + 1);
    if (!new_entry->name) {
        free(new_entry);
        printf("Error: Memory allocation failed!\n");
        return;
    }
    
    strcpy(new_entry->name, name);
    new_entry->func_ptr = func_ptr;
    new_entry->next = ht->table[index];
    ht->table[index] = new_entry;
    ht->count++;
}

double call_function(FunctionHashTable* ht, const char* name, double arg) {
    unsigned int index = hash_function(name);
    FuncEntry* current = ht->table[index];
    
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->func_ptr(arg);
        }
        current = current->next;
    }
    
    printf("Unknown function: %s\n", name);
    return 0;
}

int has_function(FunctionHashTable* ht, const char* name) {
    unsigned int index = hash_function(name);
    FuncEntry* current = ht->table[index];
    
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void list_functions(FunctionHashTable* ht) {
    printf("Available functions:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        FuncEntry* current = ht->table[i];
        while (current) {
            printf("  %s(x)\n", current->name);
            current = current->next;
        }
    }
    printf("Total: %d functions\n", ht->count);
}

void cleanup_function_hash_table(FunctionHashTable* ht) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        FuncEntry* current = ht->table[i];
        while (current) {
            FuncEntry* next = current->next;
            free(current->name);
            free(current);
            current = next;
        }
        ht->table[i] = NULL;
    }
    ht->count = 0;
}

// Built-in mathematical functions
double math_sin(double x) { return sin(x * M_PI / 180.0); }  // degrees
double math_cos(double x) { return cos(x * M_PI / 180.0); }
double math_tan(double x) { return tan(x * M_PI / 180.0); }
double math_sqrt(double x) { return sqrt(x); }
double math_log(double x) { return log10(x); }
double math_ln(double x) { return log(x); }
double math_abs(double x) { return fabs(x); }
double math_ceil(double x) { return ceil(x); }
double math_floor(double x) { return floor(x); }
double math_exp(double x) { return exp(x); }

void register_builtin_functions(FunctionHashTable* ht) {
    register_function(ht, "sin", math_sin);
    register_function(ht, "cos", math_cos);
    register_function(ht, "tan", math_tan);
    register_function(ht, "sqrt", math_sqrt);
    register_function(ht, "log", math_log);
    register_function(ht, "ln", math_ln);
    register_function(ht, "abs", math_abs);
    register_function(ht, "ceil", math_ceil);
    register_function(ht, "floor", math_floor);
    register_function(ht, "exp", math_exp);
}
