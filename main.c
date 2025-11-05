#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include "command_parser.h"
#include "expression_parser.h"
#include "evaluator.h"
#include "editor.h"
#include "bst_variables.h"
#include "hash_functions.h"

// External function declaration
extern int is_effectively_integer(double val);

// Helper function for cube root that handles negative numbers
double cbrt_real(double x) {
    if (x >= 0) {
        return pow(x, 1.0/3.0);
    } else {
        return -pow(-x, 1.0/3.0);
    }
}

VariableBST variable_bst;
FunctionHashTable function_ht;

int main() {
    char input[1024];
    TextEditor editor;
    

    init_editor(&editor);
    init_variable_bst(&variable_bst);
    init_function_hash_table(&function_ht);
    
    // Enhanced UI Header
    printf("========================================================================\n");
    printf("                    ADVANCED MATHEMATICAL TEXT EDITOR                 \n");
    printf("                          with Equation Solver                        \n");
    printf("========================================================================\n");
    printf("  Text Editor  |  Expression Evaluator  |  Equation Solver  \n");
    printf("========================================================================\n");
    printf("\n");
    printf("Welcome! Type ':help' for commands or start typing to add text.\n");
    printf("For quick start: try ':eval(2+3)', ':solve_quad(1,-5,6)', or ':solve_cubic(1,-6,11,-6)'\n\n");
    printf("MathEditor > ");

    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0; 

        if (strncmp(input, ":eval(", 6) == 0) {
            char *expr = parse_command(input);
            if (expr) {
                char postfix[1024];
                infix_to_postfix(expr, postfix);
                EvalResult result = evaluate_postfix_typed(postfix);
                
                printf(">> Expression: %s\n", expr);
                if (result.has_error) {
                    printf(">> ERROR: %s\n", result.error_message);
                } else if (result.is_integer) {
                    printf(">> Result: %d\n", (int)result.value);
                } else {
                    printf(">> Result: %.6f\n", result.value);
                }
            } else {
                printf("ERROR: Invalid eval syntax. Use :eval(expression)\n");
            }
        }
        else if (strncmp(input, ":solve_quad(", 12) == 0) {
            // Parse quadratic coefficients: :solve_quad(a, b, c)
            char *params = input + 12;
            char *end_paren = strchr(params, ')');
            if (end_paren) {
                *end_paren = '\0';
                
                // Parse three comma-separated values
                char *a_str = strtok(params, ",");
                char *b_str = strtok(NULL, ",");
                char *c_str = strtok(NULL, ",");
                
                if (a_str && b_str && c_str) {
                    // Trim whitespace
                    while (*a_str == ' ') a_str++;
                    while (*b_str == ' ') b_str++;
                    while (*c_str == ' ') c_str++;
                    
                    double a = atof(a_str);
                    double b = atof(b_str);
                    double c = atof(c_str);
                    
                    printf(">> Solving quadratic equation: %.3fx^2 + %.3fx + %.3f = 0\n", a, b, c);
                    
                    if (a == 0) {
                        printf("ERROR: Coefficient 'a' cannot be zero (not a quadratic equation)\n");
                    } else {
                        // Calculate discriminant
                        double discriminant = b * b - 4 * a * c;
                        printf(">> Discriminant (b^2 - 4ac): %.6f\n", discriminant);
                        
                        if (discriminant > 0) {
                            // Two distinct real roots
                            double sqrt_discriminant = sqrt(discriminant);
                            double root1 = (-b + sqrt_discriminant) / (2 * a);
                            double root2 = (-b - sqrt_discriminant) / (2 * a);
                            
                            printf(">> Two distinct real roots:\n");
                            if (is_effectively_integer(root1)) {
                                printf("   * x1 = %d\n", (int)root1);
                            } else {
                                printf("   * x1 = %.6f\n", root1);
                            }
                            if (is_effectively_integer(root2)) {
                                printf("   * x2 = %d\n", (int)root2);
                            } else {
                                printf("   * x2 = %.6f\n", root2);
                            }
                        } else if (discriminant == 0) {
                            // One repeated real root
                            double root = -b / (2 * a);
                            printf(">> One repeated real root:\n");
                            if (is_effectively_integer(root)) {
                                printf("   * x = %d\n", (int)root);
                            } else {
                                printf("   * x = %.6f\n", root);
                            }
                        } else {
                            // Complex roots
                            double real_part = -b / (2 * a);
                            double imaginary_part = sqrt(-discriminant) / (2 * a);
                            printf(">> Two complex roots:\n");
                            printf("   * x1 = %.6f + %.6fi\n", real_part, imaginary_part);
                            printf("   * x2 = %.6f - %.6fi\n", real_part, imaginary_part);
                        }
                    }
                } else {
                    printf("Error: Invalid syntax. Use :solve_quad(a, b, c)\n");
                }
            } else {
                printf("Error: Invalid syntax. Use :solve_quad(a, b, c)\n");
            }
        }
        else if (strncmp(input, ":solve_cubic(", 13) == 0) {
            // Parse cubic coefficients: :solve_cubic(a, b, c, d)
            char *params = input + 13;
            char *end_paren = strchr(params, ')');
            if (end_paren) {
                *end_paren = '\0';
                
                // Parse four comma-separated values
                char *a_str = strtok(params, ",");
                char *b_str = strtok(NULL, ",");
                char *c_str = strtok(NULL, ",");
                char *d_str = strtok(NULL, ",");
                
                if (a_str && b_str && c_str && d_str) {
                    // Trim whitespace
                    while (*a_str == ' ') a_str++;
                    while (*b_str == ' ') b_str++;
                    while (*c_str == ' ') c_str++;
                    while (*d_str == ' ') d_str++;
                    
                    double a = atof(a_str);
                    double b = atof(b_str);
                    double c = atof(c_str);
                    double d = atof(d_str);
                    
                    printf(">> Solving cubic equation: %.3fx^3 + %.3fx^2 + %.3fx + %.3f = 0\n", a, b, c, d);
                    
                    if (a == 0) {
                        printf("ERROR: Coefficient 'a' cannot be zero (not a cubic equation)\n");
                    } else {
                        // Convert to depressed cubic t^3 + pt + q = 0
                        // using substitution x = t - b/(3a)
                        double p = (3*a*c - b*b) / (3*a*a);
                        double q = (2*b*b*b - 9*a*b*c + 27*a*a*d) / (27*a*a*a);
                        
                        printf(">> Depressed cubic: t^3 + %.6ft + %.6f = 0\n", p, q);
                        
                        // Calculate discriminant for cubic
                        double discriminant = -(4*p*p*p + 27*q*q);
                        printf(">> Discriminant: %.6f\n", discriminant);
                        
                        if (discriminant > 0) {
                            // Three distinct real roots (Trigonometric method)
                            printf(">> Three distinct real roots:\n");
                            double m = 2 * sqrt(-p/3);
                            double theta = acos(3*q/(p*m)) / 3;
                            
                            double t1 = m * cos(theta);
                            double t2 = m * cos(theta - 2*M_PI/3);
                            double t3 = m * cos(theta - 4*M_PI/3);
                            
                            double x1 = t1 - b/(3*a);
                            double x2 = t2 - b/(3*a);
                            double x3 = t3 - b/(3*a);
                            
                            if (is_effectively_integer(x1)) {
                                printf("   * x1 = %d\n", (int)x1);
                            } else {
                                printf("   * x1 = %.6f\n", x1);
                            }
                            if (is_effectively_integer(x2)) {
                                printf("   * x2 = %d\n", (int)x2);
                            } else {
                                printf("   * x2 = %.6f\n", x2);
                            }
                            if (is_effectively_integer(x3)) {
                                printf("   * x3 = %d\n", (int)x3);
                            } else {
                                printf("   * x3 = %.6f\n", x3);
                            }
                        } else if (discriminant == 0) {
                            // Multiple roots
                            printf(">> Multiple roots:\n");
                            if (p == 0 && q == 0) {
                                // Triple root
                                double x = -b/(3*a);
                                printf("   * Triple root: x = %.6f\n", x);
                            } else {
                                // One single and one double root
                                double t1 = 3*q/p;
                                double t2 = -3*q/(2*p);
                                double x1 = t1 - b/(3*a);
                                double x2 = t2 - b/(3*a);
                                printf("   * x1 = %.6f (single)\n", x1);
                                printf("   * x2 = %.6f (double)\n", x2);
                            }
                        } else {
                            // One real root, two complex conjugate roots (Cardano's formula)
                            printf(">> One real root and two complex roots:\n");
                            double sqrt_discriminant = sqrt(-discriminant/108);
                            double u = cbrt_real(-q/2 + sqrt_discriminant);
                            double v = cbrt_real(-q/2 - sqrt_discriminant);
                            
                            double real_root = u + v - b/(3*a);
                            printf("   * Real root: x = %.6f\n", real_root);
                            
                            double real_part = -(u + v)/2 - b/(3*a);
                            double imag_part = (u - v) * sqrt(3) / 2;
                            printf("   * Complex roots: x = %.6f +/- %.6fi\n", real_part, imag_part);
                        }
                    }
                } else {
                    printf("Error: Invalid syntax. Use :solve_cubic(a, b, c, d)\n");
                }
            } else {
                printf("Error: Invalid syntax. Use :solve_cubic(a, b, c, d)\n");
            }
        }
        else if (strcmp(input, ":help") == 0) {
            printf("\n");
            printf("========================================================================\n");
            printf("                           COMMAND REFERENCE                          \n");
            printf("========================================================================\n");
            printf("                          MATHEMATICAL OPERATIONS                     \n");
            printf("========================================================================\n");
            printf("  :eval(expression)          | Evaluate mathematical expressions      \n");
            printf("                             | Examples:                              \n");
            printf("                             |   :eval(2+3*4)                        \n");
            printf("                             |   :eval(sin(45)*cos(30))              \n");
            printf("                             |   :eval(log(1000)+sqrt(16))           \n");
            printf("                             |   :eval(5!+2^8)                       \n");
            printf("                             |                                        \n");
            printf("  Supported Functions:       | sin, cos, tan, sqrt, log, ln           \n");
            printf("  Operators:                 | +, -, *, /, ^(power), !(factorial)    \n");
            printf("                             | %%(modulo), parentheses ()             \n");
            printf("========================================================================\n");
            printf("                           EQUATION SOLVERS                           \n");
            printf("========================================================================\n");
            printf("  :solve_quad(a,b,c)         | Solve quadratic equations ax^2+bx+c=0 \n");
            printf("                             | Examples:                              \n");
            printf("                             |   :solve_quad(1,-5,6)  -> x=2,3       \n");
            printf("                             |   :solve_quad(1,2,5)   -> complex     \n");
            printf("                             |                                        \n");
            printf("  :solve_cubic(a,b,c,d)      | Solve cubic equations ax^3+bx^2+cx+d=0\n");
            printf("                             | Examples:                              \n");
            printf("                             |   :solve_cubic(1,-6,11,-6) -> x=1,2,3 \n");
            printf("                             |   :solve_cubic(1,0,0,-8)   -> mixed   \n");
            printf("========================================================================\n");
            printf("                           VARIABLE MANAGEMENT                        \n");
            printf("========================================================================\n");
            printf("  :set var = value           | Store variables for calculations       \n");
            printf("  :vars                      | List all stored variables              \n");
            printf("  :clear_vars                | Clear all variables                    \n");
            printf("                             | Examples:                              \n");
            printf("                             |   :set pi = 3.14159                   \n");
            printf("                             |   :set radius = 5                     \n");
            printf("                             |   :eval(pi * radius^2)                \n");
            printf("========================================================================\n");
            printf("                           TEXT EDITOR COMMANDS                       \n");
            printf("========================================================================\n");
            printf("  :list                      | Show all lines with numbers            \n");
            printf("  :insert N text             | Insert text at line N                 \n");
            printf("  :delete N                  | Delete line N                         \n");
            printf("  :clear                     | Clear all text                        \n");
            printf("  :save filename             | Save text to file                     \n");
            printf("  :load filename             | Load text from file                   \n");
            printf("  :search text               | Search for text                       \n");
            printf("  :replace old new           | Replace text                          \n");
            printf("========================================================================\n");
            printf("                           UTILITY COMMANDS                           \n");
            printf("========================================================================\n");
            printf("  :functions                 | List available mathematical functions \n");
            printf("  :help                      | Show this help                        \n");
            printf("  :quit                      | Exit the program                      \n");
            printf("========================================================================\n");
            printf("                           TIPS & EXAMPLES                           \n");
            printf("========================================================================\n");
            printf("  * Physics: :set g=9.81; :eval(0.5*g*t^2) for motion equations     \n");
            printf("  * Finance: :eval(P*(1+r)^t) for compound interest                 \n");
            printf("  * Geometry: :eval(pi*r^2) for circle area                         \n");
            printf("  * Statistics: :eval(mean + 2*std_dev) for confidence intervals    \n");
            printf("  * Smart Display: Integer results show as integers automatically   \n");
            printf("========================================================================\n");
            printf("\n");
        }
        else if (strcmp(input, ":quit") == 0) {
            printf("\n");
            printf("========================================================================\n");
            printf("                     Thank you for using MathEditor!                  \n");
            printf("                         Cleaning up and goodbye!                     \n");
            printf("========================================================================\n");
            break;
        }

        else if (strncmp(input, ":set ", 5) == 0) {
            char* rest = input + 5;
            char* equals = strchr(rest, '=');
            if (equals) {
                *equals = '\0';
                char* var_name = rest;
                char* value_str = equals + 1;
                
                // Trim leading spaces from var_name
                while (*var_name == ' ') var_name++;
                
                // Trim trailing spaces from var_name
                char* end = var_name + strlen(var_name) - 1;
                while (end > var_name && *end == ' ') {
                    *end = '\0';
                    end--;
                }
                
                // Trim leading spaces from value_str
                while (*value_str == ' ') value_str++;
                
                double value = atof(value_str);
                set_variable_bst(&variable_bst, var_name, value);
            } else {
                printf("Usage: :set variable_name = value\n");
            }
        }
        else if (strcmp(input, ":vars") == 0) {
            list_variables_bst(&variable_bst);
        }
        else if (strcmp(input, ":clear_vars") == 0) {
            clear_variables_bst(&variable_bst);
        }
        else if (strcmp(input, ":functions") == 0) {
            list_functions(&function_ht);
        }
        else if (input[0] == ':') {
            handle_editor_command(&editor, input);
        }
        else if (strlen(input) > 0) {
            insert_line(&editor, input);
        }
        
        printf("MathEditor > ");
    }
    
    // Cleanup
    cleanup_editor(&editor);
    cleanup_variable_bst(&variable_bst);
    cleanup_function_hash_table(&function_ht);
    
    return 0;
}