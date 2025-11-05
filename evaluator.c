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
        // Handle numbers (including negative numbers)
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1])) || 
            (token[0] == '-' && strlen(token) > 1 && (isdigit(token[1]) || token[1] == '.'))) {
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
        // Handle function marker - Apply function to top stack element
        else if (token[0] == 'f' && strlen(token) == 1) {
            // This marker indicates that the previous func_ token should be applied
            // The function should already be processed by func_ token above
            continue;
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

// Helper function to check if a string represents an integer
int is_integer_string(const char* str) {
    if (!str || strlen(str) == 0) return 0;
    
    int i = 0;
    if (str[0] == '-') i = 1;  // Skip negative sign
    
    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;  // Found non-digit character
        }
    }
    return 1;
}

// Helper function to check if a double is effectively an integer
int is_effectively_integer(double val) {
    return (val == floor(val));
}

EvalResult evaluate_postfix_typed(char* postfix) {
    DoubleStack s;
    initDoubleStack(&s);
    
    EvalResult result = {0.0, 1, 0, ""};  // Start assuming integer result, no error
    
    char postfix_copy[512];
    strcpy(postfix_copy, postfix);
    
    char* token = strtok(postfix_copy, " ");
    
    while (token != NULL) {
        // Handle numbers (including negative numbers)
        if (isdigit(token[0]) || (token[0] == '.' && isdigit(token[1])) || 
            (token[0] == '-' && strlen(token) > 1 && (isdigit(token[1]) || token[1] == '.'))) {
            
            double value = atof(token);
            pushDouble(&s, value);
            
            // Check if this number is not an integer
            if (!is_integer_string(token) || !is_effectively_integer(value)) {
                result.is_integer = 0;
            }
        }
        // Handle functions - these typically produce non-integer results
        else if (strncmp(token, "func_", 5) == 0) {
            char* func_name = token + 5;
            double arg = popDouble(&s);
            
            // Check for domain errors
            if (strcmp(func_name, "sqrt") == 0 && arg < 0) {
                result.has_error = 1;
                strcpy(result.error_message, "Square root of negative number");
                result.value = 0.0;
                return result;
            }
            if ((strcmp(func_name, "log") == 0 || strcmp(func_name, "ln") == 0) && arg <= 0) {
                result.has_error = 1;
                strcpy(result.error_message, "Logarithm of non-positive number");
                result.value = 0.0;
                return result;
            }
            
            double func_result = evaluate_function_integrated(func_name, arg);
            
            // Check if function evaluation failed (NaN or infinite)
            if (isnan(func_result) || isinf(func_result)) {
                result.has_error = 1;
                sprintf(result.error_message, "Mathematical error in function %s", func_name);
                result.value = 0.0;
                return result;
            }
            
            pushDouble(&s, func_result);
            
            // Functions typically produce non-integer results
            if (!is_effectively_integer(func_result)) {
                result.is_integer = 0;
            }
        }
        // Handle constants
        else if (strncmp(token, "const_", 6) == 0) {
            char* const_name = token + 6;
            double value = get_constant(const_name);
            pushDouble(&s, value);
            
            // Constants like pi, e are typically non-integers
            if (!is_effectively_integer(value)) {
                result.is_integer = 0;
            }
        }
        // Handle variables
        else if (strncmp(token, "var_", 4) == 0) {
            char* var_name = token + 4;
            double value = get_variable_integrated(var_name);
            pushDouble(&s, value);
            
            // Variables might be non-integers
            if (!is_effectively_integer(value)) {
                result.is_integer = 0;
            }
        }
        // Handle operators
        else if (strcmp(token, "+") == 0) {
            double b = popDouble(&s);
            double a = popDouble(&s);
            pushDouble(&s, a + b);
        }
        else if (strcmp(token, "-") == 0) {
            double b = popDouble(&s);
            double a = popDouble(&s);
            pushDouble(&s, a - b);
        }
        else if (strcmp(token, "*") == 0) {
            double b = popDouble(&s);
            double a = popDouble(&s);
            pushDouble(&s, a * b);
        }
        else if (strcmp(token, "/") == 0) {
            double b = popDouble(&s);
            double a = popDouble(&s);
            
            if (b == 0.0) {
                result.has_error = 1;
                strcpy(result.error_message, "Division by zero");
                result.value = 0.0;
                return result;
            }
            
            double div_result = a / b;
            pushDouble(&s, div_result);
            
            // Division might produce non-integer result
            if (!is_effectively_integer(div_result)) {
                result.is_integer = 0;
            }
        }
        else if (strcmp(token, "^") == 0) {
            double b = popDouble(&s);
            double a = popDouble(&s);
            
            // Check for domain errors in power operations
            if (a < 0 && floor(b) != b) {
                result.has_error = 1;
                strcpy(result.error_message, "Negative base with non-integer exponent");
                result.value = 0.0;
                return result;
            }
            if (a == 0 && b < 0) {
                result.has_error = 1;
                strcpy(result.error_message, "Zero raised to negative power");
                result.value = 0.0;
                return result;
            }
            
            double pow_result = pow(a, b);
            
            // Check if power operation resulted in error
            if (isnan(pow_result) || isinf(pow_result)) {
                result.has_error = 1;
                strcpy(result.error_message, "Mathematical error in power operation");
                result.value = 0.0;
                return result;
            }
            
            pushDouble(&s, pow_result);
            
            // Power operations might produce non-integer results
            if (!is_effectively_integer(pow_result)) {
                result.is_integer = 0;
            }
        }
        else if (strcmp(token, "%") == 0) {
            double b = popDouble(&s);
            double a = popDouble(&s);
            
            if (b == 0.0) {
                result.has_error = 1;
                strcpy(result.error_message, "Modulo by zero");
                result.value = 0.0;
                return result;
            }
            
            pushDouble(&s, fmod(a, b));
            // Modulo with integers should remain integer
        }
        else if (strcmp(token, "!") == 0) {
            double a = popDouble(&s);
            
            // Check for domain errors in factorial
            if (a < 0 || floor(a) != a) {
                result.has_error = 1;
                strcpy(result.error_message, "Factorial of negative or non-integer number");
                result.value = 0.0;
                return result;
            }
            if (a > 170) {  // Factorial of 171 overflows double
                result.has_error = 1;
                strcpy(result.error_message, "Factorial too large (overflow)");
                result.value = 0.0;
                return result;
            }
            
            double factorial = 1;
            for (int i = 1; i <= (int)a; i++) {
                factorial *= i;
            }
            pushDouble(&s, factorial);
            // Factorial of integer is always integer
        }
        
        token = strtok(NULL, " ");
    }
    
    result.value = popDouble(&s);
    
    // Final check: even if all inputs were integers, the result might not be
    if (result.is_integer && !is_effectively_integer(result.value)) {
        result.is_integer = 0;
    }
    
    return result;
}
