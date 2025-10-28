// bst_variables.c - AVL BST Implementation
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bst_variables.h"


#ifndef fmax
double fmax(double a, double b) {
    return (a > b) ? a : b;
}
#endif

void init_variable_bst(VariableBST* bst) {
    bst->root = NULL;
    bst->count = 0;
}

VarNode* create_var_node(const char* name, double value) {
    VarNode* new_node = malloc(sizeof(VarNode));
    if (!new_node) {
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }
    
    new_node->name = malloc(strlen(name) + 1);
    if (!new_node->name) {
        free(new_node);
        printf("Error: Memory allocation failed!\n");
        return NULL;
    }
    
    strcpy(new_node->name, name);
    new_node->value = value;
    new_node->height = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    
    return new_node;
}

int get_height(VarNode* node) {
    return node ? node->height : 0;
}

int get_balance(VarNode* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

VarNode* rotate_right(VarNode* y) {
    VarNode* x = y->left;
    VarNode* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = 1 + fmax(get_height(y->left), get_height(y->right));
    x->height = 1 + fmax(get_height(x->left), get_height(x->right));
    
    return x;
}

VarNode* rotate_left(VarNode* x) {
    VarNode* y = x->right;
    VarNode* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = 1 + fmax(get_height(x->left), get_height(x->right));
    y->height = 1 + fmax(get_height(y->left), get_height(y->right));
    
    return y;
}

VarNode* insert_variable(VarNode* root, const char* name, double value) {
    // Standard BST insertion
    if (!root) {
        return create_var_node(name, value);
    }
    
    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        root->left = insert_variable(root->left, name, value);
    } else if (cmp > 0) {
        root->right = insert_variable(root->right, name, value);
    } else {
        // Variable exists, update value
        root->value = value;
        return root;
    }
    
    // Update height
    root->height = 1 + fmax(get_height(root->left), get_height(root->right));
    
    // Get balance factor
    int balance = get_balance(root);
    
    // AVL rotations
    // Left Left Case
    if (balance > 1 && strcmp(name, root->left->name) < 0) {
        return rotate_right(root);
    }
    
    // Right Right Case
    if (balance < -1 && strcmp(name, root->right->name) > 0) {
        return rotate_left(root);
    }
    
    // Left Right Case
    if (balance > 1 && strcmp(name, root->left->name) > 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }
    
    // Right Left Case
    if (balance < -1 && strcmp(name, root->right->name) < 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }
    
    return root;
}

VarNode* search_variable(VarNode* root, const char* name) {
    if (!root || strcmp(root->name, name) == 0) {
        return root;
    }
    
    if (strcmp(name, root->name) < 0) {
        return search_variable(root->left, name);
    } else {
        return search_variable(root->right, name);
    }
}

void inorder_traversal(VarNode* root) {
    if (root) {
        inorder_traversal(root->left);
        printf("  %s = %.6f\n", root->name, root->value);
        inorder_traversal(root->right);
    }
}

void cleanup_variable_bst_recursive(VarNode* root) {
    if (root) {
        cleanup_variable_bst_recursive(root->left);
        cleanup_variable_bst_recursive(root->right);
        free(root->name);
        free(root);
    }
}

void cleanup_variable_bst(VariableBST* bst) {
    cleanup_variable_bst_recursive(bst->root);
    bst->root = NULL;
    bst->count = 0;
}

// Public interface implementations
void set_variable_bst(VariableBST* bst, const char* name, double value) {
    VarNode* existing = search_variable(bst->root, name);
    if (!existing) {
        bst->count++;
    }
    bst->root = insert_variable(bst->root, name, value);
    printf("Variable %s set to %.6f\n", name, value);
}

double get_variable_bst(VariableBST* bst, const char* name) {
    VarNode* node = search_variable(bst->root, name);
    if (node) {
        return node->value;
    } else {
        printf("Undefined variable: %s\n", name);
        return 0;
    }
}

int has_variable_bst(VariableBST* bst, const char* name) {
    return search_variable(bst->root, name) != NULL;
}

void list_variables_bst(VariableBST* bst) {
    if (bst->count == 0) {
        printf("No variables defined.\n");
        return;
    }
    
    printf("Defined variables (sorted):\n");
    inorder_traversal(bst->root);
    printf("Total: %d variables\n", bst->count);
}

void clear_variables_bst(VariableBST* bst) {
    cleanup_variable_bst(bst);
    printf("All variables cleared.\n");
}
