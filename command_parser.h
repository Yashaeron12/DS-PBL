#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

char* parse_command(char* input);
int is_eval_command(char* input);
int is_editor_command(char* input);
char* extract_eval_expression(char* input);

#endif
