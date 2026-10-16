#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insert_non_full(Node* node, const char* key, const char* value) {
    int i = node->key_count - 1;

    if (node->is_leaf) {
        while (i >= 0 && strcmp(key, node->keys[i]) < 0) {
            node->keys[i + 1] = node->keys[i];
            node->info[i + 1] = node->info[i];
            i--;
        }
        node->keys[i + 1] = strdup_safe(key);
        node->info[i + 1] = strdup_safe(value);
        node->key_count++;
    } else {
        while (i >= 0 && strcmp(key, node->keys[i]) < 0) i--;
        i++;

        if (node->children[i]->key_count == 3) {
            split_child(&node, i);
            if (strcmp(key, node->keys[i]) > 0) i++;
        }
        insert_non_full(node->children[i], key, value);
    }
}

int btree_insert(BTree* tree, const char* key, const char* value) {
    if (!tree) return -1;

    if (btree_search(tree, key) != NULL) {
        printf("Ошибка: ключ '%s' уже существует в дереве.\n", key);
        return -1;
    }

    if (!tree->root) {
        tree->root = create_node(1);
    }

    if (tree->root->key_count == 3) {
        Node* new_root = create_node(0);
        new_root->children[0] = tree->root;
        tree->root = new_root;
        split_child(&new_root, 0);
    }

    insert_non_full(tree->root, key, value);
    return 0;
}
