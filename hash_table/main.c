#include "func.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    printf("Размер таблицы: ");
    int size;
    if (check_input(&size, 1) != 1) return 1;
    Table *table = new_table(size);
    if (!table) {
        printf("Ошибка создания таблицы\n");
        return 1;
    }
    int choice;
    char *filename = NULL;
    int key, info;
    KeySpace *found = NULL;
    while (1) {
        printf("\nМеню:\n");
        printf("1. Вставить\n");
        printf("2. Удалить\n");
        printf("3. Найти по ключу\n");
        printf("4. Вывести таблицу\n");
        printf("5. Импорт из файла\n");
        printf("6. Экспорт в файл\n");
        printf("0. Выход\n");
        printf("> ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                printf("Введите ключ: ");
                if (check_input(&key, 0) != 1) {
                    free_table(table);
                    return 1;
                }
                printf("Введите информацию: ");
                if (check_input(&info, 0) != 1) {
                    free_table(table);
                    return 1;
                }
                int insert_result = insert(table, key, info);
                if (insert_result == 0)
                    printf("Успешно добавлено\n");
                else if (insert_result == -2)
                    printf("Ошибка добавления: ключ уже существует\n");
                else if (insert_result == -3)
                    printf("Ошибка добавления: таблица переполнена\n");
                else
                    printf("Ошибка добавления\n");
                break;
            case 2:
                printf("Введите ключ: ");
                if (check_input(&key, 0) != 1) {
                    free_table(table);
                    return 1;
                }
                if (delet(table, key) == 0)
                    printf("Успешно удалено\n");
                else
                    printf("Ошибка удаления\n");
                break;
            case 3:
                printf("Введите ключ: ");
                if (check_input(&key, 0) != 1) {
                    free_table(table);
                    return 1;
                }
                found = find_by_key(table, key);
                if (found && found->busy == 1)
                    printf("Найдено:\nKey: %u\nInfo: %u\n", found->key, *(found->info));
                else
                    printf("Не найдено\n");
                break;
            case 4:
                print_table(table);
                break;
            case 5:
                printf("Введите имя файла: ");
                filename = my_readline(stdin);
                if (filename) {
                    if (import_from_file(&table, filename) == 0)
                        printf("Импорт успешен\n");
                    else
                        printf("Ошибка с файлом\n");
                } else
                    printf("Ошибка чтения имени файла\n");
                free(filename);
                filename = NULL;
                break;
            case 6:
                printf("Введите имя файла для экспорта: ");
                filename = my_readline(stdin);
                if (filename) {
                    if (export_to_file(table, filename) == 0)
                        printf("Экспорт успешен\n");
                    else
                        printf("Ошибка экспорта\n");
                } else
                    printf("Ошибка чтения имени файла\n");
                free(filename);
                filename = NULL;
                break;
            case 0:
                free_table(table);
                return 0;
            default:
                printf("Неверный выбор\n");
        }
    }
}
