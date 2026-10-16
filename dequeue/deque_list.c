#include "deque.h"

typedef struct Node {
    Patient patient;
    struct Node* next;
} Node;

struct Deque {
    Node* head;
    Node* tail;
};

Deque* deque_create(int capacity) {
    Deque* deque = malloc(sizeof(Deque));
    if (!deque) return NULL;

    deque->head = deque->tail = NULL;
    return deque;
}

void free_deque(Deque* deque) {
    if (!deque) return;

    Node* current = deque->head;
    while (current) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(deque);
}

int add_head(Deque* deque, const char* id, int ta, int ts) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) return -1;

    snprintf(newNode->patient.id, sizeof(newNode->patient.id), "%s", id);
    newNode->patient.ta = ta;
    newNode->patient.ts = ts;
    newNode->next = deque->head;

    if (!deque->head) deque->tail = newNode;
    deque->head = newNode;
    return 0;
}

int add_tail(Deque* deque, const char* id, int ta, int ts) {
    Node* newNode = malloc(sizeof(Node));
    if (!newNode) return -1;

    snprintf(newNode->patient.id, sizeof(newNode->patient.id), "%s", id);
    newNode->patient.ta = ta;
    newNode->patient.ts = ts;
    newNode->next = NULL;

    if (!deque->head) {
        deque->head = deque->tail = newNode;
    } else {
        deque->tail->next = newNode;
        deque->tail = newNode;
    }
    return 0;
}

int dell_head(Deque* deque, char* id, int* ta, int* ts) {
    if (deque_is_empty(deque)) return -1;

    Node* temp = deque->head;
    strcpy(id, temp->patient.id);
    *ta = temp->patient.ta;
    *ts = temp->patient.ts;

    deque->head = temp->next;
    if (!deque->head) deque->tail = NULL;
    free(temp);
    return 0;
}

int dell_tail(Deque* deque) {
    if (deque_is_empty(deque)) return -1;

    if (deque->head == deque->tail) {
        free(deque->head);
        deque->head = deque->tail = NULL;
        return 0;
    }

    Node* current = deque->head;
    while (current->next != deque->tail) {
        current = current->next;
    }

    free(deque->tail);
    deque->tail = current;
    deque->tail->next = NULL;
    return 0;
}

int deque_is_empty(const Deque* deque) {
    return deque->head == NULL;
}

void deque_print(Deque* deque, int time) {
    printf("%d\n", time);
    if (deque_is_empty(deque)) {
        printf("\n");
        return;
    }

    Node* current = deque->head;
    while (current) {
        printf("%s ", current->patient.id);
        current = current->next;
    }
    printf("\n");
}
