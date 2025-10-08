#include <string.h>
#include "command_parser.h"


char* parse_command(char* input) {
char* expr = input + 6; // after ":eval("
size_t len = strlen(expr);
if (len > 0 && expr[len - 1] == ')') {
expr[len - 1] = '\0'; // remove trailing ')'
}
return expr;
}