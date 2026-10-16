#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_key_index(Node* node, const char* key) {
    int index = 0;
    while (index < node->key_count && strcmp(key, node->keys[index]) > 0) {
        index++;
    }
    return index;
}

const char* get_predecessor(Node* node, int index) {
    Node* current = node->children[index];
    while (!current->is_leaf) {
        current = current->children[current->key_count];
    }
    return current->info[current->key_count - 1];
}

const char* get_successor(Node* node, int index) {
    Node* current = node->children[index + 1];
    while (!current->is_leaf) {
        current = current->children[0];
    }
    return current->info[0];
}

void borrow_from_left(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* sibling = parent->children[index - 1];

    for (int i = child->key_count; i > 0; i--) {
        child->keys[i] = child->keys[i - 1];
        child->info[i] = child->info[i - 1];
    }
    if (!child->is_leaf) {
        for (int i = child->key_count + 1; i > 0; i--) {
            child->children[i] = child->children[i - 1];
        }
    }

    child->keys[0] = parent->keys[index - 1];
    child->info[0] = parent->info[index - 1];
    parent->keys[index - 1] = sibling->keys[sibling->key_count - 1];
    parent->info[index - 1] = sibling->info[sibling->key_count - 1];

    if (!sibling->is_leaf) {
        child->children[0] = sibling->children[sibling->key_count];
    }

    child->key_count++;
    sibling->key_count--;
}

void borrow_from_right(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* sibling = parent->children[index + 1];

    child->keys[child->key_count] = parent->keys[index];
    child->info[child->key_count] = parent->info[index];
    parent->keys[index] = sibling->keys[0];
    parent->info[index] = sibling->info[0];

    for (int i = 1; i < sibling->key_count; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
        sibling->info[i - 1] = sibling->info[i];
    }

    if (!sibling->is_leaf) {
        child->children[child->key_count + 1] = sibling->children[0];
        for (int i = 1; i <= sibling->key_count; i++) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->key_count++;
    sibling->key_count--;
}

void merge_nodes(Node* parent, int index) {
    Node* child = parent->children[index];
    Node* sibling = parent->children[index + 1];

    child->keys[child->key_count] = parent->keys[index];
    child->info[child->key_count] = parent->info[index];

    for (int i = 0; i < sibling->key_count; i++) {
        child->keys[child->key_count + 1 + i] = sibling->keys[i];
        child->info[child->key_count + 1 + i] = sibling->info[i];
    }

    if (!child->is_leaf) {
        for (int i = 0; i <= sibling->key_count; i++) {
            child->children[child->key_count + 1 + i] = sibling->children[i];
        }
    }

    for (int i = index; i < parent->key_count - 1; i++) {
        parent->keys[i] = parent->keys[i + 1];
        parent->info[i] = parent->info[i + 1];
        parent->children[i + 1] = parent->children[i + 2];
    }

    child->key_count += sibling->key_count + 1;
    parent->key_count--;

    free(sibling->keys);
    free(sibling->info);
    free(sibling->children);
    free(sibling);
}

void delete_key(Node* root, const char* key) {
    if (!root) return;

    int index = find_key_index(root, key);

    if (index < root->key_count && strcmp(key, root->keys[index]) == 0) {
        if (root->is_leaf) {
            for (int i = index; i < root->key_count - 1; i++) {
                root->keys[i] = root->keys[i + 1];
                root->info[i] = root->info[i + 1];
            }
            root->key_count--;
        } else {
            if (root->children[index]->key_count >= 2) {
                const char* predecessor = get_predecessor(root, index);
                root->keys[index] = strdup_safe(predecessor);
                delete_key(root->children[index], predecessor);
            } else if (root->children[index + 1]->key_count >= 2) {
                const char* successor = get_successor(root, index);
                root->keys[index] = strdup_safe(successor);
                delete_key(root->children[index + 1], successor);
            } else {
                merge_nodes(root, index);
                delete_key(root->children[index], key);
            }
        }
    } else {
        if (root->is_leaf) {
            return;
        }

        int flag = (index == root->key_count);

        if (root->children[index]->key_count < 2) {
            if (index != 0 && root->children[index - 1]->key_count >= 2) {
                borrow_from_left(root, index);
            } else if (index != root->key_count && root->children[index + 1]->key_count >= 2) {
                borrow_from_right(root, index);
            } else {
                if (index == root->key_count) {
                    merge_nodes(root, index - 1);
                } else {
                    merge_nodes(root, index);
                }
            }
        }

        delete_key(root->children[index], key);
    }

    if (root->key_count == 0) {
        Node* temp = root;
        root = root->children[0];

        free(temp->keys);
        free(temp->info);
        free(temp->children);
        free(temp);
    }
}

int btree_delete(BTree* tree, const char* key) {
    if (!tree || !tree->root) return -1;

    delete_key(tree->root, key);

    if (tree->root->key_count == 0) {
        Node* temp = tree->root;
        tree->root = tree->root->children[0];

        free(temp->keys);
        free(temp->info);
        free(temp->children);
        free(temp);
    }

    return 0;
}
