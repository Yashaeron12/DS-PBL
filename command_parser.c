#include <string.h>
#include <stdlib.h>
#include "command_parser.h"

char* parse_command(char* input) {
    // Extract expression from :eval(expression)
    char* start = strchr(input, '(');
    char* end = strrchr(input, ')');
    if (start && end && start < end) {
        start++; 
        *end = '\0'; 
        return start;
    }
    return NULL;
}
int is_eval_command(char* input) {
    return strncmp(input, ":eval(", 6) == 0;
}
int is_editor_command(char* input) {
    return input[0] == ':';
}
char* extract_eval_expression(char* input) {
    return parse_command(input);
}
