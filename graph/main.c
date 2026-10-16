#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Graph* graph = createGraph(100);
    int choice;
    char name1[MAX_NAME_LEN], name2[MAX_NAME_LEN];
    int weight;

    do {
        printf("\nМеню:\n");
        printf("1. Добавить вершину\n");
        printf("2. Удалить вершину\n");
        printf("3. Добавить ребро\n");
        printf("4. Удалить ребро\n");
        printf("5. Вывести граф\n");
        printf("6. Обход графа (BFS)\n");
        printf("7. Поиск кратчайшего пути (Bellman-Ford)\n");
        printf("8. Поиск лучшей цепочки (Floyd-Warshall)\n");
        printf("9. Графический вывод графа (Graphviz)\n");
        printf("10. Изменить данные вершины\n");
        printf("11. Изменить данные ребра\n");
        printf("0. Выход\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите имя человека: ");
                scanf("%s", name1);
                if (addVertex(graph, name1) == 0) {
                    printf("Вершина добавлена.\n");
                } else {
                    printf("Ошибка при добавлении вершины.\n");
                }
                break;
            case 2:
                printf("Введите имя человека для удаления: ");
                scanf("%s", name1);
                if (removeVertex(graph, name1) == 0) {
                    printf("Вершина удалена.\n");
                } else {
                    printf("Ошибка при удалении вершины.\n");
                }
                break;
            case 3:
                printf("Введите начальную вершину: ");
                scanf("%s", name1);
                printf("Введите конечную вершину: ");
                scanf("%s", name2);
                printf("Введите вес ребра (-10..10): ");
                scanf("%d", &weight);
                if (addEdge(graph, name1, name2, weight) == 0) {
                    printf("Ребро добавлено.\n");
                } else {
                    printf("Ошибка при добавлении ребра.\n");
                }
                break;
            case 4:
                printf("Введите начальную вершину: ");
                scanf("%s", name1);
                printf("Введите конечную вершину: ");
                scanf("%s", name2);
                if (removeEdge(graph, name1, name2) == 0) {
                    printf("Ребро удалено.\n");
                } else {
                    printf("Ошибка при удалении ребра.\n");
                }
                break;
            case 5:
                printAdjacencyLists(graph);
                break;
            case 6:
                printf("Введите начальную вершину для обхода: ");
                scanf("%s", name1);
                bfsPotentialFriends(graph, name1);
                break;
            case 7:
                printf("Введите начальную вершину: ");
                scanf("%s", name1);
                printf("Введите конечную вершину: ");
                scanf("%s", name2);
                bellmanFordShortestPath(graph, name1, name2);
                break;
            case 8:
                printf("Введите начальную вершину: ");
                scanf("%s", name1);
                floydWarshallBestChain(graph, name1);
                break;
            case 9:
                generateDotFile(graph, "graph.dot");

                FILE* dotFile = fopen("graph.dot", "r");
                if (!dotFile) {
                    printf("Ошибка: Файл 'graph.dot' не найден.\n");
                    break;
                }
                fclose(dotFile);

                int result = system("dot -Tpng graph.dot -o graph.png");
                if (result != 0) {
                    printf("Ошибка: Не удалось создать файл 'graph.png'. Убедитесь, что Graphviz установлен.\n");
                    break;
                }

                result = system("xdg-open graph.png 2>/dev/null");
                if (result != 0) {
                    printf("Файл 'graph.png' создан, но не удалось открыть его. Попробуйте открыть вручную.\n");
                } else {
                    printf("Файл 'graph.png' успешно открыт.\n");
                }
                break;
            case 10:
                printf("Введите старое имя вершины: ");
                scanf("%s", name1);
                printf("Введите новое имя вершины: ");
                scanf("%s", name2);
                if (changeVertexName(graph, name1, name2) == 0) {
                    printf("Имя вершины успешно изменено.\n");
                } else {
                    printf("Ошибка при изменении имени вершины.\n");
                }
                break;
            case 11:
                printf("Введите начальную вершину: ");
                scanf("%s", name1);
                printf("Введите конечную вершину: ");
                scanf("%s", name2);
                printf("Введите новый вес ребра (-10..10): ");
                scanf("%d", &weight);
                if (changeEdgeWeight(graph, name1, name2, weight) == 0) {
                    printf("Вес ребра успешно изменен.\n");
                } else {
                    printf("Ошибка при изменении веса ребра.\n");
                }
                break;
            case 0:
                printf("Выход из программы.\n");
                break;
            default:
                printf("Неверный выбор. Попробуйте снова.\n");
        }
    } while (choice != 0);

    destroyGraph(graph);
    return 0;
}
