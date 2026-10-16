#include "func.h"
#include <limits.h>

unsigned int hash(unsigned int key) {
    char buffer[32];
    int i = 0;
    do {
        buffer[i++] = (key % 10) + '0';
        key /= 10;
    } while (key > 0);
    buffer[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
    unsigned int h1 = 0;
    for (int j = 0; buffer[j] != '\0'; j++) h1 += 101 * buffer[j];
    unsigned int h2 = 1;
    for (int j = 0; buffer[j] != '\0'; j++) h2 *= 37 * buffer[j];
    return (h1 + h2) % UINT_MAX;
}

int check_input(int *num, int min) {
    int check = 0;
    do {
        check = scanf("%d", num);
        if (check == EOF) return -1;
        if (check == 1 && *num > min) return 1;
        printf("Повторите ввод: ");
        scanf("%*[^\n]");
    } while (check != 1 || *num < min);
    return 1;
}

char *my_readline(FILE *fptr) {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do {
        n = fscanf(fptr, "%80[^\n]", buf);
        if (n < 0) {
            if (!res) return NULL;
        } else if (n == 1) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else if (n == 0) {
            fscanf(fptr, "%*c");
        }
    } while (n > 0);
    if (len > 0) res[len] = '\0';
    else res = calloc(1, sizeof(char));
    return res;
}

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2 || n == 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 4; i * i <= n; i += 1)
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    return 1;
}

int nearest(int num) {
    int n = num;
    while (!is_prime(n)) n++;
    return n;
}

Table *new_table(int size) {
    Table *table = malloc(sizeof(Table));
    if (!table) return NULL;
    size = nearest(size);
    table->msize = size;
    table->ks = malloc(sizeof(KeySpace) * size);
    if (!table->ks) {
        free(table);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        table->ks[i].busy = 0;
        table->ks[i].info = malloc(sizeof(unsigned int));
        if (!table->ks[i].info) {
            free_table(table);
            return NULL;
        }
        *(table->ks[i].info) = 0;
        table->ks[i].key = 0;
    }
    return table;
}

int free_table(Table *table) {
    if (!table) return -1;
    for (int i = 0; i < table->msize; i++) free(table->ks[i].info);
    free(table->ks);
    free(table);
    return 0;
}

int print_table(const Table *table) {
    if (!table) return -1;
    for (int i = 0; i < table->msize; i++) {
        printf("Busy: %d\nKey: %u\nInfo: %u\n---\n", table->ks[i].busy, table->ks[i].key, *(table->ks[i].info));
    }
    return 0;
}

KeySpace *find_by_key(const Table *table, const unsigned int key) {
    if (!table) return NULL;
    unsigned int index = hash(key) % table->msize;
    return &(table->ks[index]);
}

unsigned int hash2(unsigned int key, int msize) {
    return (key % (msize - 1)) + 1;
}

int insert(Table *table, unsigned int key, unsigned int info) {
    if (!table) return -1;
    int full = 1;
    for (int i = 0; i < table->msize; i++) if (!table->ks[i].busy) full = 0;
    if (full) return -3;
    unsigned int index = hash(key) % table->msize;
    unsigned int step = hash2(key, table->msize);
    while (table->ks[index].busy == 1) {
        if (table->ks[index].key == key) return -2;
        index = (index + step) % table->msize;
    }
    table->ks[index].busy = 1;
    *(table->ks[index].info) = info;
    table->ks[index].key = key;
    return 0;
}

int delet(Table *table, unsigned int key) {
    if (!table) return -1;
    unsigned int index = hash(key) % table->msize;
    unsigned int step = hash2(key, table->msize);
    while (table->ks[index].busy == 1) {
        if (table->ks[index].key == key) {
            table->ks[index].busy = 0;
            return 0;
        }
        index = (index + step) % table->msize;
    }
    return -2;
}

int import_from_file(Table **table, const char *filename) {
    if (!table || !filename) return -1;
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) return -1;
    int msize;
    fread(&msize, sizeof(int), 1, fptr);
    Table *new_table_ptr = new_table(msize);
    if (!new_table_ptr) {
        fclose(fptr);
        return -1;
    }
    for (int i = 0; i < msize; i++) {
        fread(&new_table_ptr->ks[i].busy, sizeof(char), 1, fptr);
        fread(&new_table_ptr->ks[i].key, sizeof(unsigned int), 1, fptr);
        fread(new_table_ptr->ks[i].info, sizeof(unsigned int), 1, fptr);
    }
    fclose(fptr);
    if (*table) free_table(*table);
    *table = new_table_ptr;
    return 0;
}

int export_to_file(const Table *table, const char *filename) {
    if (!table || !filename) return -1;
    FILE *fptr = fopen(filename, "wb");
    if (!fptr) return -1;
    fwrite(&table->msize, sizeof(int), 1, fptr);
    for (int i = 0; i < table->msize; i++) {
        fwrite(&table->ks[i].busy, sizeof(char), 1, fptr);
        fwrite(&table->ks[i].key, sizeof(unsigned int), 1, fptr);
        fwrite(table->ks[i].info, sizeof(unsigned int), 1, fptr);
    }
    fclose(fptr);
    return 0;
}
