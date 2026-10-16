#ifndef DEQUE_H
#define DEQUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id[100];
    int ta;
    int ts;
} Patient;

typedef struct Deque Deque;

Deque* deque_create(int capacity);
void free_deque(Deque* deque);
int add_head(Deque* deque, const char* id, int ta, int ts);
int add_tail(Deque* deque, const char* id, int ta, int ts);
int dell_head(Deque* deque, char* id, int* ta, int* ts);
int dell_tail(Deque* deque);
int deque_is_empty(const Deque* deque);
void deque_print(Deque* deque, int time);

#endif
