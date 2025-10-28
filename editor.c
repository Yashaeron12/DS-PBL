// editor.c - LINKED LIST IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "editor.h"

void init_editor(TextEditor* editor) {
    editor->head = NULL;           //  LINKED LIST
    editor->tail = NULL;           //  LINKED LIST
    editor->current = NULL;        //  LINKED LIST
    editor->total_lines = 0;
    editor->max_line_length = 1024;
}

void cleanup_editor(TextEditor* editor) {
    LineNode* current = editor->head;
    while (current) {              //  TRAVERSE LINKED LIST
        LineNode* next = current->next;
        free(current->content);    //  DYNAMIC MEMORY
        free(current);             //  DYNAMIC MEMORY
        current = next;
    }
    init_editor(editor);
}

LineNode* create_line_node(const char* content, int line_number) {
    LineNode* new_node = malloc(sizeof(LineNode));  //  DYNAMIC ALLOCATION
    if (!new_node) {
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }
    
    new_node->content = malloc(strlen(content) + 1);  //  DYNAMIC SIZE
    if (!new_node->content) {
        free(new_node);
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }
    
    strcpy(new_node->content, content);
    new_node->line_number = line_number;
    new_node->next = NULL;         //  LINKED LIST POINTERS
    new_node->prev = NULL;         //  DOUBLY LINKED
    
    return new_node;
}

void insert_line(TextEditor* editor, const char* text) {
    LineNode* new_node = create_line_node(text, editor->total_lines + 1);
    if (!new_node) return;
    
    if (editor->head == NULL) {
        // First line -  LINKED LIST LOGIC
        editor->head = editor->tail = new_node;
    } else {
        // Append to end -  LINKED LIST INSERTION
        editor->tail->next = new_node;
        new_node->prev = editor->tail;
        editor->tail = new_node;
    }
    
    editor->total_lines++;
    editor->current = new_node;
    printf("Line %d added: %s\n", editor->total_lines, text);
}

void insert_line_at_position(TextEditor* editor, const char* text, int position) {
    if (position < 1 || position > editor->total_lines + 1) {
        printf("Error: Invalid position %d\n", position);
        return;
    }
    
    if (position == editor->total_lines + 1) {
        insert_line(editor, text);
        return;
    }
    
    LineNode* new_node = create_line_node(text, position);
    if (!new_node) return;
    
    if (position == 1) {
        // Insert at beginning
        new_node->next = editor->head;
        if (editor->head) {
            editor->head->prev = new_node;
        }
        editor->head = new_node;
        if (!editor->tail) {
            editor->tail = new_node;
        }
    } else {
        // Insert in middle
        LineNode* current = get_line_by_number(editor, position);
        if (current) {
            new_node->next = current;
            new_node->prev = current->prev;
            if (current->prev) {
                current->prev->next = new_node;
            }
            current->prev = new_node;
        }
    }
    
    // Update line numbers LINKED LIST TRAVERSAL
    LineNode* update_node = new_node->next;
    while (update_node) {
        update_node->line_number++;
        update_node = update_node->next;
    }
    
    editor->total_lines++;
    editor->current = new_node;
    printf("Line inserted at position %d\n", position);
}

LineNode* get_line_by_number(TextEditor* editor, int line_num) {
    if (line_num < 1 || line_num > editor->total_lines) {
        return NULL;
    }
    
    LineNode* current;
    
    //  OPTIMIZED LINKED LIST SEARCH
    if (line_num <= editor->total_lines / 2) {
        // Start from head
        current = editor->head;
        for (int i = 1; i < line_num && current; i++) {
            current = current->next;
        }
    } else {
        // Start from tail (doubly linked advantage)
        current = editor->tail;
        for (int i = editor->total_lines; i > line_num && current; i--) {
            current = current->prev;
        }
    }
    
    return current;
}

void delete_line(TextEditor* editor, int line_num) {
    LineNode* to_delete = get_line_by_number(editor, line_num);
    if (!to_delete) {
        printf("Error: Line %d not found!\n", line_num);
        return;
    }
    
    //  LINKED LIST DELETION
    if (to_delete->prev) {
        to_delete->prev->next = to_delete->next;
    } else {
        editor->head = to_delete->next;
    }
    
    if (to_delete->next) {
        to_delete->next->prev = to_delete->prev;
    } else {
        editor->tail = to_delete->prev;
    }
    
    // Update line numbers  LINKED LIST TRAVERSAL
    LineNode* update_node = to_delete->next;
    while (update_node) {
        update_node->line_number--;
        update_node = update_node->next;
    }
    
    //  DYNAMIC MEMORY CLEANUP
    free(to_delete->content);
    free(to_delete);
    editor->total_lines--;
    
    printf("Line %d deleted.\n", line_num);
}

void edit_line(TextEditor* editor, int line_num, const char* new_text) {
    LineNode* line = get_line_by_number(editor, line_num);
    if (!line) {
        printf("Error: Line %d not found!\n", line_num);
        return;
    }
    
    //  DYNAMIC MEMORY REALLOCATION
    if (strlen(new_text) > strlen(line->content)) {
        free(line->content);
        line->content = malloc(strlen(new_text) + 1);
        if (!line->content) {
            printf("Error: Memory allocation failed!\n");
            return;
        }
    }
    
    strcpy(line->content, new_text);
    editor->current = line;
    printf("Line %d updated.\n", line_num);
}

void display_editor(TextEditor* editor) {
    printf("\n--- Text Editor Content (LINKED LIST) ---\n");
    if (editor->total_lines == 0) {
        printf("(Empty document)\n");
    } else {
        LineNode* current = editor->head;  //  LINKED LIST TRAVERSAL
        while (current) {
            char marker = (current == editor->current) ? '*' : ' ';
            printf("%c%3d: %s\n", marker, current->line_number, current->content);
            current = current->next;        //  LINKED LIST NAVIGATION
        }
    }
    printf("--- End of Document ---\n");
    printf("Lines: %d | Memory: Dynamic (Linked List)\n\n", editor->total_lines);
}

void save_to_file(TextEditor* editor, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Cannot save to file '%s'\n", filename);
        return;
    }
    
    LineNode* current = editor->head;  //  LINKED LIST TRAVERSAL
    while (current) {
        fprintf(file, "%s\n", current->content);
        current = current->next;
    }
    
    fclose(file);
    printf("Document saved to '%s' (%d lines)\n", filename, editor->total_lines);
}

void load_from_file(TextEditor* editor, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }
    
    cleanup_editor(editor);  // Clear current content
    
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        insert_line(editor, line);  //  USES LINKED LIST
    }
    
    fclose(file);
    printf("Document loaded from '%s' (%d lines)\n", filename, editor->total_lines);
}

void show_help() {
    printf("\n=== Console based text with expression evaluation ===\n");
    printf("Text input:         Type normally to add lines\n");
    printf(":show               Display all lines with current line marker\n");
    printf(":insert <n> <text>  Insert text at line number n\n");
    printf(":delete <n>         Delete line number n\n");
    printf(":edit <n> <text>    Edit line n with new text\n");
    printf(":goto <n>           Go to line number n\n");
    printf(":save <file>        Save to file\n");
    printf(":load <file>        Load from file\n");
    printf(":clear              Clear all text\n");
    printf(":help               Show this help\n");
    printf(":quit               Exit editor\n");
    printf("\n=== Mathematical Evaluation ===\n");
    printf(":eval(expr)         Evaluate mathematical expression\n");
    printf("Functions:          sin, cos, tan, sqrt, log, ln, abs, ceil, floor, exp\n");
    printf("Constants:          pi, e, phi\n");
    printf("Variables:          Use any name (x, y, myvar)\n");
    printf(":set var = val      Set variable value (stored in BST)\n");
    printf(":vars               List all variables (sorted by BST)\n");
    printf(":clear_vars         Clear all variables\n");
    printf(":functions          List all functions (from hash table)\n");
    printf("=============================\n\n");
}

void handle_editor_command(TextEditor* editor, const char* command) {
    if (strcmp(command, ":show") == 0) {
        display_editor(editor);
    }
    else if (strcmp(command, ":clear") == 0) {
        cleanup_editor(editor);
        printf("Document cleared.\n");
    }
    else if (strcmp(command, ":help") == 0) {
        show_help();
    }
    else if (strncmp(command, ":insert ", 8) == 0) {
        char* rest = (char*)command + 8;
        char* space = strchr(rest, ' ');
        if (space) {
            *space = '\0';
            int position = atoi(rest);
            char* text = space + 1;
            insert_line_at_position(editor, text, position);
        } else {
            printf("Usage: :insert <position> <text>\n");
        }
    }
    else if (strncmp(command, ":delete ", 8) == 0) {
        int line_num = atoi(command + 8);
        delete_line(editor, line_num);
    }
    else if (strncmp(command, ":edit ", 6) == 0) {
        char* rest = (char*)command + 6;
        char* space = strchr(rest, ' ');
        if (space) {
            *space = '\0';
            int line_num = atoi(rest);
            char* new_text = space + 1;
            edit_line(editor, line_num, new_text);
        } else {
            printf("Usage: :edit <line_number> <new_text>\n");
        }
    }
    else if (strncmp(command, ":goto ", 6) == 0) {
        int line_num = atoi(command + 6);
        LineNode* line = get_line_by_number(editor, line_num);
        if (line) {
            editor->current = line;
            printf("Moved to line %d: %s\n", line_num, line->content);
        } else {
            printf("Error: Line %d not found!\n", line_num);
        }
    }
    else if (strncmp(command, ":save ", 6) == 0) {
        const char* filename = command + 6;
        save_to_file(editor, filename);
    }
    else if (strncmp(command, ":load ", 6) == 0) {
        const char* filename = command + 6;
        load_from_file(editor, filename);
    }
    else {
        printf("Unknown command. Type :help for available commands.\n");
    }
}
