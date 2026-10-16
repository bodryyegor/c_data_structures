#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NAME_LEN 50
#define RELATIONSHIP_MIN -10
#define RELATIONSHIP_MAX 10

typedef struct Edge {
    int vertex;
    int weight;
    struct Edge* next;
} Edge;

typedef struct Vertex {
    char name[MAX_NAME_LEN];
    Edge* edges;
} Vertex;

typedef struct Graph {
    Vertex* vertices;
    int size;
    int capacity;
} Graph;

Graph* createGraph(int capacity);
void destroyGraph(Graph* graph);
int addVertex(Graph* graph, const char* name);
int removeVertex(Graph* graph, const char* name);
int addEdge(Graph* graph, const char* from, const char* to, int weight);
int removeEdge(Graph* graph, const char* from, const char* to);
void printAdjacencyLists(const Graph* graph);
void bfsPotentialFriends(const Graph* graph, const char* start);
void bellmanFordShortestPath(const Graph* graph, const char* start, const char* end);
void floydWarshallBestChain(const Graph* graph, const char* start);
void generateDotFile(const Graph* graph, const char* filename);
int changeVertexName(Graph* graph, const char* oldName, const char* newName);
int changeEdgeWeight(Graph* graph, const char* from, const char* to, int newWeight);

#endif
