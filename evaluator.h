#ifndef EVALUATOR_H
#define EVALUATOR_H

typedef struct {
    double value;
    int is_integer;  // 1 if result should be displayed as integer, 0 otherwise
    int has_error;   // 1 if there was an error, 0 otherwise
    char error_message[256];  // Error message if has_error is 1
} EvalResult;

double evaluate_postfix(char* postfix);
EvalResult evaluate_postfix_typed(char* postfix);

#endif