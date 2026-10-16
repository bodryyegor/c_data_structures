#include "graph.h"

Graph* createGraph(int capacity) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = (Vertex*)malloc(capacity * sizeof(Vertex));
    graph->size = 0;
    graph->capacity = capacity;
    return graph;
}

void destroyGraph(Graph* graph) {
    for (int i = 0; i < graph->size; i++) {
        Edge* edge = graph->vertices[i].edges;
        while (edge) {
            Edge* temp = edge;
            edge = edge->next;
            free(temp);
        }
    }
    free(graph->vertices);
    free(graph);
}

int addVertex(Graph* graph, const char* name) {
    if (graph->size >= graph->capacity) {
        return -1;
    }
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            return -2;
        }
    }
    strcpy(graph->vertices[graph->size].name, name);
    graph->vertices[graph->size].edges = NULL;
    graph->size++;
    return 0;
}

int removeVertex(Graph* graph, const char* name) {
    int index = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return -1;
    }

    for (int i = 0; i < graph->size; i++) {
        Edge* prev = NULL;
        Edge* curr = graph->vertices[i].edges;
        while (curr) {
            if (curr->vertex == index) {
                if (prev) {
                    prev->next = curr->next;
                } else {
                    graph->vertices[i].edges = curr->next;
                }
                Edge* temp = curr;
                curr = curr->next;
                free(temp);
            } else {
                if (curr->vertex > index) {
                    curr->vertex--;
                }
                prev = curr;
                curr = curr->next;
            }
        }
    }

    Edge* edge = graph->vertices[index].edges;
    while (edge) {
        Edge* temp = edge;
        edge = edge->next;
        free(temp);
    }
    for (int i = index; i < graph->size - 1; i++) {
        graph->vertices[i] = graph->vertices[i + 1];
    }
    graph->size--;
    return 0;
}

int addEdge(Graph* graph, const char* from, const char* to, int weight) {
    if (weight < RELATIONSHIP_MIN || weight > RELATIONSHIP_MAX) {
        return -1;
    }
    int fromIndex = -1, toIndex = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, from) == 0) {
            fromIndex = i;
        }
        if (strcmp(graph->vertices[i].name, to) == 0) {
            toIndex = i;
        }
    }
    if (fromIndex == -1 || toIndex == -1) {
        return -2;
    }

    Edge* edge = graph->vertices[fromIndex].edges;
    while (edge) {
        if (edge->vertex == toIndex) {
            return -3;
        }
        edge = edge->next;
    }

    Edge* newEdge = (Edge*)malloc(sizeof(Edge));
    newEdge->vertex = toIndex;
    newEdge->weight = weight;
    newEdge->next = graph->vertices[fromIndex].edges;
    graph->vertices[fromIndex].edges = newEdge;
    return 0;
}

int removeEdge(Graph* graph, const char* from, const char* to) {
    int fromIndex = -1, toIndex = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, from) == 0) {
            fromIndex = i;
        }
        if (strcmp(graph->vertices[i].name, to) == 0) {
            toIndex = i;
        }
    }
    if (fromIndex == -1 || toIndex == -1) {
        return -1;
    }

    Edge* prev = NULL;
    Edge* curr = graph->vertices[fromIndex].edges;
    while (curr) {
        if (curr->vertex == toIndex) {
            if (prev) {
                prev->next = curr->next;
            } else {
                graph->vertices[fromIndex].edges = curr->next;
            }
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -2;
}

void printAdjacencyLists(const Graph* graph) {
    for (int i = 0; i < graph->size; i++) {
        printf("%s -> ", graph->vertices[i].name);
        Edge* edge = graph->vertices[i].edges;
        while (edge) {
            printf("%s(%d) ", graph->vertices[edge->vertex].name, edge->weight);
            edge = edge->next;
        }
        printf("\n");
    }
}

void bfsPotentialFriends(const Graph* graph, const char* start) {
    int startIdx = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, start) == 0) {
            startIdx = i;
            break;
        }
    }
    if (startIdx == -1) {
        printf("Вершина '%s' не найдена.\n", start);
        return;
    }

    int visited[graph->size];
    for (int i = 0; i < graph->size; i++) {
        visited[i] = 0;
    }

    int queue[graph->size];
    int front = 0, rear = 0;
    queue[rear++] = startIdx;
    visited[startIdx] = 1;

    printf("Потенциальные друзья для '%s':\n", start);
    while (front != rear) {
        int current = queue[front++];
        Edge* edge = graph->vertices[current].edges;
        while (edge) {
            if (!visited[edge->vertex] && edge->weight >= 0) {
                visited[edge->vertex] = 1;
                queue[rear++] = edge->vertex;
                printf("%s ", graph->vertices[edge->vertex].name);
            }
            edge = edge->next;
        }
    }
    printf("\n");
}

void bellmanFordShortestPath(const Graph* graph, const char* start, const char* end) {
    int startIdx = -1, endIdx = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, start) == 0) {
            startIdx = i;
        }
        if (strcmp(graph->vertices[i].name, end) == 0) {
            endIdx = i;
        }
    }
    if (startIdx == -1 || endIdx == -1) {
        printf("Одна из вершин не найдена.\n");
        return;
    }

    int distances[graph->size];
    for (int i = 0; i < graph->size; i++) {
        distances[i] = INT_MAX;
    }
    distances[startIdx] = 0;

    for (int i = 0; i < graph->size - 1; i++) {
        for (int j = 0; j < graph->size; j++) {
            Edge* edge = graph->vertices[j].edges;
            while (edge) {
                if (distances[j] != INT_MAX && distances[j] + edge->weight < distances[edge->vertex]) {
                    distances[edge->vertex] = distances[j] + edge->weight;
                }
                edge = edge->next;
            }
        }
    }

    if (distances[endIdx] == INT_MAX) {
        printf("Путь от '%s' до '%s' не существует.\n", start, end);
    } else {
        printf("Кратчайший путь от '%s' до '%s': %d\n", start, end, distances[endIdx]);
    }
}

void floydWarshallBestChain(const Graph* graph, const char* start) {
    int dist[graph->size][graph->size];
    for (int i = 0; i < graph->size; i++) {
        for (int j = 0; j < graph->size; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = INT_MIN;
            }
        }
    }

    for (int i = 0; i < graph->size; i++) {
        Edge* edge = graph->vertices[i].edges;
        while (edge) {
            dist[i][edge->vertex] = edge->weight;
            edge = edge->next;
        }
    }

    for (int k = 0; k < graph->size; k++) {
        for (int i = 0; i < graph->size; i++) {
            for (int j = 0; j < graph->size; j++) {
                if (dist[i][k] != INT_MIN && dist[k][j] != INT_MIN) {
                    if (dist[i][k] + dist[k][j] > dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    int startIdx = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, start) == 0) {
            startIdx = i;
            break;
        }
    }
    if (startIdx == -1) {
        printf("Вершина '%s' не найдена.\n", start);
        return;
    }

    int maxSum = INT_MIN;
    int bestFriend = -1;
    for (int i = 0; i < graph->size; i++) {
        if (i != startIdx && dist[startIdx][i] > maxSum) {
            maxSum = dist[startIdx][i];
            bestFriend = i;
        }
    }

    if (bestFriend == -1) {
        printf("Нет потенциальных друзей для '%s'.\n", start);
    } else {
        printf("Лучшая цепочка для '%s': %s (суммарная оценка: %d)\n",
               start, graph->vertices[bestFriend].name, maxSum);
    }
}

void generateDotFile(const Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка: Не удалось создать файл '%s'.\n", filename);
        return;
    }

    fprintf(file, "digraph G {\n");
    for (int i = 0; i < graph->size; i++) {
        Edge* edge = graph->vertices[i].edges;
        while (edge) {
            fprintf(file, "    \"%s\" -> \"%s\" [label=\"%d\"];\n",
                    graph->vertices[i].name,
                    graph->vertices[edge->vertex].name,
                    edge->weight);
            edge = edge->next;
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    printf("DOT-файл '%s' успешно создан.\n", filename);
}

int changeVertexName(Graph* graph, const char* oldName, const char* newName) {
    int index = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, oldName) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return -1;
    }
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, newName) == 0) {
            return -2;
        }
    }
    strcpy(graph->vertices[index].name, newName);
    return 0;
}

int changeEdgeWeight(Graph* graph, const char* from, const char* to, int newWeight) {
    if (newWeight < RELATIONSHIP_MIN || newWeight > RELATIONSHIP_MAX) {
        return -1;
    }
    int fromIndex = -1, toIndex = -1;
    for (int i = 0; i < graph->size; i++) {
        if (strcmp(graph->vertices[i].name, from) == 0) {
            fromIndex = i;
        }
        if (strcmp(graph->vertices[i].name, to) == 0) {
            toIndex = i;
        }
    }
    if (fromIndex == -1 || toIndex == -1) {
        return -2;
    }

    Edge* edge = graph->vertices[fromIndex].edges;
    while (edge) {
        if (edge->vertex == toIndex) {
            edge->weight = newWeight;
            return 0;
        }
        edge = edge->next;
    }
    return -3;
}
