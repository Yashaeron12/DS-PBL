#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command_parser.h"
#include "expression_parser.h"
#include "evaluator.h"
#include "editor.h"
#include "bst_variables.h"
#include "hash_functions.h"

// Global data structures
VariableBST variable_bst;
FunctionHashTable function_ht;

int main() {
    char input[1024];
    TextEditor editor;
    
    // Initialize all data structures
    init_editor(&editor);
    init_variable_bst(&variable_bst);
    init_function_hash_table(&function_ht);
    
    printf("=== Enhanced Text Editor with Advanced Data Structures ===\n");
    printf("Features: Linked Lists, BST Variables, Hash Functions\n");
    printf("Type :help for commands\n\n");
    printf("Enhanced Editor > ");

    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = 0; // remove newline

        if (strncmp(input, ":eval(", 6) == 0) {
            char *expr = parse_command(input);
            if (expr) {
                char postfix[1024];
                infix_to_postfix(expr, postfix);
                double result = evaluate_postfix(postfix);
                printf("Result: %.6f\n", result);
            } else {
                printf("Error: Invalid eval syntax. Use :eval(expression)\n");
            }
        }
        else if (strcmp(input, ":quit") == 0) {
            printf("Cleaning up and goodbye!\n");
            break;
        }
        // Variable commands (now using BST)
        else if (strncmp(input, ":set ", 5) == 0) {
            char* rest = input + 5;
            char* equals = strchr(rest, '=');
            if (equals) {
                *equals = '\0';
                char* var_name = rest;
                char* value_str = equals + 1;
                
                // Trim whitespace
                while (*var_name == ' ') var_name++;
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
            // FIXED: Remove undo_mgr parameter
            handle_editor_command(&editor, input);
        }
        else if (strlen(input) > 0) {
            // FIXED: Use insert_line instead of append_line
            insert_line(&editor, input);
        }
        
        printf("Enhanced Editor > ");
    }
    
    // Cleanup
    cleanup_editor(&editor);
    cleanup_variable_bst(&variable_bst);
    cleanup_function_hash_table(&function_ht);
    
    return 0;
}
