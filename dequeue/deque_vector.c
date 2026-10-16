#include "deque.h"

struct Deque {
    Patient* patients;
    int head;
    int tail;
    int size;
    int capacity;
};

Deque* deque_create(int capacity) {
    Deque* deque = malloc(sizeof(Deque));
    if (!deque) return NULL;

    deque->patients = calloc(capacity, sizeof(Patient));
    if (!deque->patients) {
        free(deque);
        return NULL;
    }

    deque->head = 0;
    deque->tail = 0;
    deque->size = 0;
    deque->capacity = capacity;
    return deque;
}

void free_deque(Deque* deque) {
    if (!deque) return;
    free(deque->patients);
    free(deque);
}

int add_head(Deque* deque, const char* id, int ta, int ts) {
    if (!deque || deque->size == deque->capacity) return -1;

    deque->head = (deque->head - 1 + deque->capacity) % deque->capacity;

    strncpy(deque->patients[deque->head].id, id, sizeof(deque->patients[0].id)-1);
    deque->patients[deque->head].id[sizeof(deque->patients[0].id)-1] = '\0';
    deque->patients[deque->head].ta = ta;
    deque->patients[deque->head].ts = ts;

    deque->size++;
    return 0;
}

int add_tail(Deque* deque, const char* id, int ta, int ts) {
    if (!deque || deque->size == deque->capacity) return -1;

    strncpy(deque->patients[deque->tail].id, id, sizeof(deque->patients[0].id)-1);
    deque->patients[deque->tail].id[sizeof(deque->patients[0].id)-1] = '\0';
    deque->patients[deque->tail].ta = ta;
    deque->patients[deque->tail].ts = ts;

    deque->tail = (deque->tail + 1) % deque->capacity;
    deque->size++;
    return 0;
}

int dell_head(Deque* deque, char* id, int* ta, int* ts) {
    if (deque_is_empty(deque)) return -1;

    strcpy(id, deque->patients[deque->head].id);
    *ta = deque->patients[deque->head].ta;
    *ts = deque->patients[deque->head].ts;
    deque->head = (deque->head + 1) % deque->capacity;
    deque->size--;
    return 0;
}

int dell_tail(Deque* deque) {
    if (deque_is_empty(deque)) return -1;

    deque->tail = (deque->tail - 1 + deque->capacity) % deque->capacity;
    deque->size--;
    return 0;
}

int deque_is_empty(const Deque* deque) {
    return deque->size == 0;
}

void deque_print(Deque* deque, int time) {
    printf("%d\n", time);
    if (deque_is_empty(deque)) {
        printf("\n");
        return;
    }

    int index = deque->head;
    for (int i = 0; i < deque->size; i++) {
        printf("%s ", deque->patients[index].id);
        index = (index + 1) % deque->capacity;
    }
    printf("\n");
}
