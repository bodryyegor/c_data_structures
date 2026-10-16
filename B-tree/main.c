#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    BTree tree = {NULL};
    int choice;
    char *key = NULL, *info = NULL, *filename = NULL;
    size_t len = 0;

    while (1) {
        printf("\nМеню:\n");
        printf("1. Вставить\n");
        printf("2. Удалить\n");
        printf("3. Обход дерева\n");
        printf("4. Поиск по ключу\n");
        printf("5. Специальный поиск\n");
        printf("6. Загрузка из файла\n");
        printf("7. Вывод дерева\n");
        printf("0. Выход\n");
        printf("> ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Введите ключ: ");
                getline(&key, &len, stdin);
                key[strcspn(key, "\n")] = '\0';
                printf("Введите информацию: ");
                getline(&info, &len, stdin);
                info[strcspn(info, "\n")] = '\0';
                if (btree_insert(&tree, key, info) == 0) {
                    printf("Успешно добавлено\n");
                } else {
                    printf("Ошибка добавления\n");
                }
                break;
            case 2:
                printf("Введите ключ: ");
                getline(&key, &len, stdin);
                key[strcspn(key, "\n")] = '\0';
                if (btree_delete(&tree, key) == 0) {
                    printf("Успешно удалено\n");
                } else {
                    printf("Ошибка удаления\n");
                }
                break;
            case 3:
                printf("Обход дерева:\n");
                btree_traverse(&tree);
                break;
            case 4:
                printf("Введите ключ: ");
                getline(&key, &len, stdin);
                key[strcspn(key, "\n")] = '\0';
                const char *result = btree_search(&tree, key);
                if (result) {
                    printf("Найдено: %s\n", result);
                } else {
                    printf("Не найдено\n");
                }
                break;
            case 5:
                printf("Введите ключ: ");
                getline(&key, &len, stdin);
                key[strcspn(key, "\n")] = '\0';
                const char *closest = btree_special_search(&tree, key);
                if (closest) {
                    printf("Ближайший ключ: %s\n", closest);
                } else {
                    printf("Не найдено\n");
                }
                break;
            case 6:
                printf("Введите имя файла: ");
                getline(&filename, &len, stdin);
                filename[strcspn(filename, "\n")] = '\0';
                if (btree_load_from_file(&tree, filename) == 0) {
                    printf("Импорт успешен\n");
                } else {
                    printf("Ошибка с файлом\n");
                }
                break;
            case 7:
                btree_print(&tree);
                break;
            case 0:
                btree_free(&tree);
                free(key);
                free(info);
                free(filename);
                return 0;
            default:
                printf("Неверный выбор\n");
        }
    }
}
