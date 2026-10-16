#ifndef FUNC_H
#define FUNC_H

typedef struct Node {
    char** keys;
    char** info;
    struct Node** children;
    int key_count;
    int is_leaf;
} Node;

typedef struct {
    Node* root;
} BTree;

char* strdup_safe(const char* str);
Node* create_node(int is_leaf);
void split_child(Node** parent, int index);
int btree_insert(BTree* tree, const char* key, const char* value);
int btree_delete(BTree* tree, const char* key);
void btree_traverse(BTree* tree);
const char* btree_search(BTree* tree, const char* key);
const char* btree_special_search(BTree* tree, const char* key);
int btree_load_from_file(BTree* tree, const char* filename);
void btree_print(BTree* tree);
void btree_free(BTree* tree);

#endif
