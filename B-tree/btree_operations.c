#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* strdup_safe(const char* str) {
    if (!str) return NULL;
    char* copy = malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

Node* create_node(int is_leaf) {
    Node* node = malloc(sizeof(Node));
    node->keys = calloc(3, sizeof(char*));
    node->info = calloc(3, sizeof(char*));
    node->children = calloc(4, sizeof(Node*));
    node->key_count = 0;
    node->is_leaf = is_leaf;
    return node;
}

void split_child(Node** parent, int index) {
    Node* child = (*parent)->children[index];
    Node* new_node = create_node(child->is_leaf);

    for (int i = 0; i < 1; i++) {
        new_node->keys[i] = child->keys[i + 2];
        new_node->info[i] = child->info[i + 2];
        child->keys[i + 2] = NULL;
        child->info[i + 2] = NULL;
    }
    if (!child->is_leaf) {
        for (int i = 0; i < 2; i++) {
            new_node->children[i] = child->children[i + 2];
            child->children[i + 2] = NULL;
        }
    }

    for (int i = (*parent)->key_count; i > index; i--) {
        (*parent)->keys[i] = (*parent)->keys[i - 1];
        (*parent)->info[i] = (*parent)->info[i - 1];
        (*parent)->children[i + 1] = (*parent)->children[i];
    }

    (*parent)->keys[index] = child->keys[1];
    (*parent)->info[index] = child->info[1];
    (*parent)->children[index + 1] = new_node;

    (*parent)->key_count++;
    child->key_count = 1;
}

void free_node(Node* node) {
    if (!node) return;

    for (int i = 0; i < node->key_count; i++) {
        free(node->keys[i]);
        free(node->info[i]);
    }
    if (!node->is_leaf) {
        for (int i = 0; i <= node->key_count; i++) {
            free_node(node->children[i]);
        }
    }
    free(node->keys);
    free(node->info);
    free(node->children);
    free(node);
}

void btree_free(BTree* tree) {
    free_node(tree->root);
    tree->root = NULL;
}

const char* search_by_key(Node* root, const char* key) {
    if (!root) return NULL;

    int i = 0;
    while (i < root->key_count && strcmp(key, root->keys[i]) > 0) i++;

    if (i < root->key_count && strcmp(key, root->keys[i]) == 0) {
        return root->info[i];
    }

    if (root->is_leaf) return NULL;

    return search_by_key(root->children[i], key);
}

const char* btree_search(BTree* tree, const char* key) {
    return search_by_key(tree->root, key);
}

const char* special_search(Node* root, const char* key) {
    if (!root) return NULL;

    int i = 0;
    while (i < root->key_count && strcmp(key, root->keys[i]) > 0) i++;

    if (i < root->key_count && strcmp(key, root->keys[i]) == 0) {
        return root->info[i];
    }

    if (root->is_leaf) {
        if (i > 0) {
            return root->info[i - 1];
        } else {
            return NULL;
        }
    }

    if (i == root->key_count) {
        return special_search(root->children[i], key);
    }

    return special_search(root->children[i], key);
}

const char* btree_special_search(BTree* tree, const char* key) {
    return special_search(tree->root, key);
}

void traverse_tree(Node* root) {
    if (!root) return;

    for (int i = 0; i < root->key_count; i++) {
        if (!root->is_leaf) {
            traverse_tree(root->children[i]);
        }
        printf("Key: %s, Value: %s\n", root->keys[i], root->info[i]);
    }
    if (!root->is_leaf) {
        traverse_tree(root->children[root->key_count]);
    }
}

void btree_traverse(BTree* tree) {
    traverse_tree(tree->root);
}

void print_tree_with_indentation(Node* root, int level, int is_last) {
    if (!root) return;

    for (int i = 0; i < level - 1; i++) {
        printf("│   ");
    }

    if (level > 0) {
        if (is_last) {
            printf("└── ");
        } else {
            printf("├── ");
        }
    }

    for (int i = 0; i < root->key_count; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%s", root->keys[i]);
    }
    printf("\n");

    if (!root->is_leaf) {
        for (int i = 0; i <= root->key_count; i++) {
            int is_last_child = (i == root->key_count);
            print_tree_with_indentation(root->children[i], level + 1, is_last_child);
        }
    }
}

void btree_print(BTree* tree) {
    if (!tree || !tree->root) {
        printf("(empty)\n");
        return;
    }
    print_tree_with_indentation(tree->root, 0, 0);
}

int btree_load_from_file(BTree* tree, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return -1;
    }

    char key[256], value[256];
    while (fgets(key, sizeof(key), file) && fgets(value, sizeof(value), file)) {
        key[strcspn(key, "\n")] = '\0';
        value[strcspn(value, "\n")] = '\0';
        btree_insert(tree, key, value);
    }

    fclose(file);
    return 0;
}
