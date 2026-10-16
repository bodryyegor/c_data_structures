# Text Word Search with a 2-3-4 Tree

A C project that implements a 2-3-4 tree (a balanced search tree also known as a (2,4)-tree) and uses it as the core data structure for a fast word search utility over text files.

Both keys and values are null-terminated strings of arbitrary length. The tree does not allow duplicate keys: inserting an existing key updates its value and returns the old one.

The project ships with a reusable library (`func.c` / `func.h`) and an interactive command-line program (`main.c`) that lets the user work with the tree and run word searches across files.

## Features

- Balanced 2-3-4 tree with ordered keys.
- Keys and values: null-terminated strings of arbitrary length.
- No duplicate keys: insertion of an existing key replaces the value.
- Full set of tree operations:
  - insertion,
  - deletion,
  - traversal (in reverse order of keys greater than a given one),
  - search by key,
  - special search: the element with the largest key not exceeding a given one.
- Pretty-printed tree output.
- Graphviz (`dot`) export for graphical visualization.
- Loading a tree from a plain text file.
- Fast word search over text files (the applied task).
- Interactive command-line interface.
- No global variables; clean memory management; suitable for Valgrind and AddressSanitizer.

## Project layout

```text
.
├── func.h          # Public interface of the 2-3-4 tree and word search
├── func.c          # Implementation of the tree and search logic
└── main.c          # Interactive command-line program
```

## Building

No Makefile is included. Compile the project directly with your C compiler.

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c func.c -o tree_search
```

Debug build with sanitizers:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined main.c func.c -o tree_search_asan
```

## Running

```bash
./tree_search
```

The program reads commands from standard input and writes results to standard output.

## Tree structure

Each node of the 2-3-4 tree stores:

- an array of keys (up to 3 keys),
- an array of pointers to child subtrees (up to 4 children),
- a pointer to the information field (the value),
- optionally, a pointer to the parent node.

```c
#define MAX_KEYS 3
#define MAX_CHILDREN 4

typedef struct Node {
    char *keys[MAX_KEYS];
    char *values[MAX_KEYS];
    struct Node *children[MAX_CHILDREN];
    struct Node *parent;
    int n; /* current number of keys */
} Node;
```

All strings are dynamically allocated and owned by the tree.


## Applied task: fast word search

The tree is used as an index over words found in text files.

- Key: the word (string).
- Value: a location string in the form `<filename>:<line_number>:<offset_in_line>`.
- Duplicates are not allowed, so the stored value always points to the **first occurrence** of the word.

Lookups are O(log n) thanks to the balanced 2-3-4 tree.

### Example

```text
> load book.txt
Indexed 4821 words from book.txt
> find tree
book.txt:17:12
> find algorithm
book.txt:204:31
```

## Interactive commands

| Command | Description |
|---------|-------------|
| `insert <key> <value>` | Insert a key/value pair. Updates the value if the key already exists. |
| `delete <key>` | Remove the element with the given key. |
| `search <key>` | Print the value associated with the key. |
| `special <key>` | Find the element with the largest key not exceeding the given one. |
| `traverse [key]` | Traverse the tree in reverse order of keys greater than `key` (or the whole tree if no key is given). |
| `print` | Pretty-print the tree structure. |
| `dot <filename>` | Export the tree in Graphviz DOT format. |
| `load <filename>` | Load a tree from a text file. |
| `index <filename>` | Index all words of a text file into the tree. |
| `find <word>` | Search for a word indexed earlier; prints `file:line:offset`. |
| `help` | Show available commands. |
| `exit` | Quit the program. |

## Loading format

A tree can be loaded from a plain text file with alternating lines:

```text
<key1>
<value1>
<key2>
<value2>
...
```

Empty lines are skipped, and the parser verifies that every key has a matching value.

## Graphviz output

Use the `dot` command to produce a `.dot` file, then render it with Graphviz:

```bash
dot -Tpng tree.dot -o tree.png
```

## Implementation notes

### Balance

- The 2-3-4 tree is kept perfectly balanced by construction.
- Insertion splits full nodes as it descends from the root.
- Deletion uses standard 2-3-4 rebalancing (merge / borrow from siblings).

### Complexity

| Operation | Time |
|-----------|------|
| Insert    | O(log n) |
| Delete    | O(log n) |
| Search    | O(log n) |
| Traversal | O(n) |
| Special search | O(log n) |

### Memory management

- Every key, value, and node is dynamically allocated.
- `tree_destroy` recursively releases the whole tree.
- The program avoids global variables and is intended to be clean under Valgrind and sanitizers.

### Error handling

- Functions return error codes; the interactive shell prints a human-readable message and keeps running.
- All user input is validated before it reaches the library.

## License

MIT License. See `LICENSE` if included.
