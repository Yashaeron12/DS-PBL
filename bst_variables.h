// bst_variables.h - BST for Variable Storage
#ifndef BST_VARIABLES_H
#define BST_VARIABLES_H

typedef struct VarNode {
    char* name;
    double value;
    int height;  // For AVL balancing
    struct VarNode* left;
    struct VarNode* right;
} VarNode;

typedef struct {
    VarNode* root;
    int count;
} VariableBST;

// BST operations
void init_variable_bst(VariableBST* bst);
void cleanup_variable_bst(VariableBST* bst);
VarNode* create_var_node(const char* name, double value);
VarNode* insert_variable(VarNode* root, const char* name, double value);
VarNode* search_variable(VarNode* root, const char* name);
VarNode* delete_variable(VarNode* root, const char* name);
void inorder_traversal(VarNode* root);
int get_height(VarNode* node);
int get_balance(VarNode* node);
VarNode* rotate_right(VarNode* y);
VarNode* rotate_left(VarNode* x);

// Public interface
void set_variable_bst(VariableBST* bst, const char* name, double value);
double get_variable_bst(VariableBST* bst, const char* name);
int has_variable_bst(VariableBST* bst, const char* name);
void list_variables_bst(VariableBST* bst);
void clear_variables_bst(VariableBST* bst);

#endif
