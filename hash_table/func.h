#ifndef FUNC_H
#define FUNC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct KeySpace {
    char busy;
    unsigned int key;
    unsigned int *info;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int msize;
} Table;

unsigned int hash(unsigned int);
int check_input(int *, int);
char *my_readline(FILE *);
Table *new_table(int);
int free_table(Table *);
int print_table(const Table *);
KeySpace *find_by_key(const Table *, const unsigned int);
int insert(Table *, unsigned int, unsigned int);
int delet(Table *, unsigned int);
int import_from_file(Table **, const char *);
int export_to_file(const Table *, const char *);

#endif
