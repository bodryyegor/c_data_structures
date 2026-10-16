# Social Graph Explorer

A C project that implements a directed graph and uses it to model a **circle of acquaintances**. Each vertex is a person, each edge stores a relationship rating between two people. The library provides traversal, shortest path, and “special” path queries, plus all the usual graph editing operations.

The project ships with a reusable library (`graph.c` / `graph.h`) and an interactive command-line program (`main.c`) that lets the user build, edit, visualize, and query the graph.

## Features

- Directed graph stored as adjacency lists.
- Vertices hold a unique person name (fixed-size string, up to `MAX_NAME_LEN`).
- Edges hold an integer relationship rating in the range `[-10, 10]`.
- Full set of editing operations:
  - add / remove vertex,
  - add / remove edge,
  - change a vertex name,
  - change an edge weight.
- Traversal:
  - BFS over “non-negative” chains — people who could potentially become friends.
- Shortest path:
  - Bellman–Ford over edge weights, minimizing the total relationship rating.
- Special operation:
  - Floyd–Warshall to find the person reachable with the **maximum** total relationship rating.
- Human-readable adjacency-list output.
- Graphviz (`dot`) export for graphical visualization.
- Clean memory management; suitable for Valgrind and AddressSanitizer.
- No global variables.

## Project layout

```text
.
├── graph.h         # Public interface of the graph library
├── graph.c         # Implementation of the graph and its algorithms
└── main.c          # Interactive command-line program
```

## Building

No Makefile is included. Compile the project directly with your C compiler.

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c graph.c -o social_graph
```

Debug build with sanitizers:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined main.c graph.c -o social_graph_asan
```

## Running

```bash
./social_graph
```

Or with an input file:

```bash
./social_graph < commands.txt
```

The program reads commands from standard input and writes results to standard output.

## Graph structure

The graph is stored as adjacency lists. Each vertex keeps:

- a name (`char[MAX_NAME_LEN]`),
- a head pointer to its outgoing edge list.

Each edge keeps:

- the destination vertex index,
- the relationship rating (int, `-10..10`),
- a next pointer for the adjacency list.

```c
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
```

The vertex array is dynamically allocated once with a fixed `capacity` and is owned by the graph.

## Public API

The public interface is defined in `graph.h`:

```c
Graph* createGraph(int capacity);
void   destroyGraph(Graph* graph);

int addVertex(Graph* graph, const char* name);
int removeVertex(Graph* graph, const char* name);

int addEdge(Graph* graph, const char* from, const char* to, int weight);
int removeEdge(Graph* graph, const char* from, const char* to);

int changeVertexName(Graph* graph, const char* oldName, const char* newName);
int changeEdgeWeight(Graph* graph, const char* from, const char* to, int newWeight);

void printAdjacencyLists(const Graph* graph);
void generateDotFile(const Graph* graph, const char* filename);

void bfsPotentialFriends(const Graph* graph, const char* start);
void bellmanFordShortestPath(const Graph* graph, const char* start, const char* end);
void floydWarshallBestChain(const Graph* graph, const char* start);
```

All library functions take explicit parameters and return error codes (`0` on success, non-zero on failure). None of them is dialog-based; the interactive shell lives entirely in `main.c`.

## Supported operations

### Traversal — potential friends (BFS)

Starting from a given person, the BFS walks through the directed graph but only follows edges whose rating is **non-negative**. The result is the list of people reachable through chains where nobody in the chain has a negative relationship toward the next person.

### Shortest path — Bellman–Ford

Given two people, the algorithm finds the chain of acquaintances with the **minimum total** relationship rating. Negative edges are allowed, which is exactly why Bellman–Ford is used instead of Dijkstra.

### Special operation — Floyd–Warshall (maximum total rating)

Given a person, find the other person reachable through a chain of acquaintances with the **maximum total** relationship rating. Implemented with the Floyd–Warshall all-pairs formulation.

## Interactive commands

| Command | Description |
|---------|-------------|
| `add_vertex <name>` | Add a new person. |
| `remove_vertex <name>` | Remove a person and all incident edges. |
| `rename <old> <new>` | Change a person’s name. |
| `add_edge <from> <to> <weight>` | Add a directed edge with the given weight (`-10..10`). |
| `remove_edge <from> <to>` | Remove the edge. |
| `change_weight <from> <to> <weight>` | Update the edge weight. |
| `print` | Print the graph as adjacency lists. |
| `dot <filename>` | Export the graph in Graphviz DOT format. |
| `friends <name>` | BFS: people who could potentially be friends. |
| `path <from> <to>` | Bellman–Ford: minimum total weight path. |
| `best <name>` | Floyd–Warshall: person reachable with maximum total rating. |
| `help` | Show available commands. |
| `exit` | Quit the program. |

## Graphviz output

Use the `dot` command to produce a `.dot` file, then render it:

```bash
dot -Tpng graph.dot -o graph.png
```

Edge ratings are shown as labels; negative edges can be rendered in a different color for clarity.

## Complexity

Let `V` be the number of vertices and `E` the number of edges.

| Operation | Time |
|-----------|------|
| Add / remove vertex | O(V + E) worst case (reindexing) |
| Add / remove edge | O(deg(v)) |
| BFS | O(V + E) |
| Bellman–Ford | O(V · E) |
| Floyd–Warshall | O(V³) |
| Print adjacency lists | O(V + E) |

Non-recursive algorithms are preferred throughout the implementation.

## Implementation notes

- Adjacency lists are used at the physical level, as required.
- The algorithms are implemented iteratively (no recursion), so deep chains will not overflow the stack.
- The graph owns the vertex array and all edge nodes; `destroyGraph` releases everything.
- The program validates all user input and reports errors without terminating.
- No global variables are used.

## License

MIT License. See `LICENSE` if included.
