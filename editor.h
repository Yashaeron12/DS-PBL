#ifndef EDITOR_H
#define EDITOR_H

#include "utils.h"

// Linked List Node for text lines
typedef struct LineNode {
    char* content;
    int line_number;
    struct LineNode* next;
    struct LineNode* prev;
} LineNode;

//  LINKED LIST
typedef struct {
    LineNode* head;
    LineNode* tail;
    LineNode* current;
    int total_lines;
    int max_line_length;
} TextEditor;

// Core editor functions
void init_editor(TextEditor* editor);
void cleanup_editor(TextEditor* editor);
void display_editor(TextEditor* editor);
LineNode* create_line_node(const char* content, int line_number);
void insert_line(TextEditor* editor, const char* text);  // This function exists
void insert_line_at_position(TextEditor* editor, const char* text, int position);
void delete_line(TextEditor* editor, int line_num);
void edit_line(TextEditor* editor, int line_num, const char* new_text);
LineNode* get_line_by_number(TextEditor* editor, int line_num);
void save_to_file(TextEditor* editor, const char* filename);
void load_from_file(TextEditor* editor, const char* filename);
void show_help();

// Corrected: Function signature matches main.c usage
void handle_editor_command(TextEditor* editor, const char* command);

#endif
