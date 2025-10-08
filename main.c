#include <stdio.h>
#include <string.h>
#include "command_parser.h"
#include "expression_parser.h"
#include "evaluator.h"


int main() {
char input[256];
printf("Mini Text Editor with Eval > ");


while (fgets(input, sizeof(input), stdin)) {
input[strcspn(input, "\n")] = 0; // remove newline


if (strncmp(input, ":eval(", 6) == 0) {
char *expr = parse_command(input);
char postfix[256];


infix_to_postfix(expr, postfix);
double result = evaluate_postfix(postfix);
printf("Result: %.2f\n", result);
}
else if (strcmp(input, ":quit") == 0) {
break;
}
else {
printf("[TEXT] %s\n", input);
}
printf("Mini Text Editor with Eval > ");
}
return 0;
}