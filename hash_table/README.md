# Hash Table with Double Hashing

A small C project that implements a fixed-capacity hash table backed by a vector and uses double hashing for collision resolution. The table stores unsigned integer keys and unsigned integer values.

The project includes a reusable library (`func.c` / `func.h`) and an interactive command-line program (`main.c`) that lets the user work with the table: insert, delete, search, print, import from a binary file, export to a binary file, and automatically resize when the table becomes full.

## Features

- Hash table based on a contiguous vector of key slots.
- Double hashing for open addressing.
- Unique keys enforced.
- Fixed initial capacity set at initialization.
- Automatic resizing when the table is full (load factor 1).
  - New size is automatically chosen as a prime number.
  - All existing elements are rehashed.
- Import and export of the entire table state to/from a binary file.
- Interactive command-line interface.
- Clean memory management; suitable for Valgrind and AddressSanitizer.
- No global variables.

## Project layout

```text
.
├── func.h          # Public interface of the hash table
├── func.c          # Implementation of the hash table
└── main.c          # Interactive command-line program
```

## Building

No Makefile is included. Compile the project directly with your C compiler.

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c func.c -o hash_table
```

Debug build with sanitizers:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined main.c func.c -o hash_table_asan
```

## Running

```bash
./hash_table
```

The program reads commands from standard input and writes results to standard output.

## Table structure

The table is represented by a `Table` structure. Internally each slot is a `KeySpace` element:

```c
typedef struct KeySpace {
    BusyType busy;   /* occupancy flag */
    KeyType key;     /* unsigned integer key */
    InfoType *info;  /* pointer to unsigned integer value */
} KeySpace;

typedef struct Table {
    KeySpace *ks;    /* vector of key slots */
    IndexType msize; /* maximum number of slots */
    IndexType csize; /* current number of elements */
} Table;
```

- `KeyType` — unsigned integer.
- `InfoType` — unsigned integer.
- `BusyType` — occupancy flag (e.g., `0` for empty, `1` for occupied).

## Interactive commands

The program supports the following commands:

| Command | Description |
|---------|-------------|
| `insert <key> <value>` | Insert a new element. Fails if the key already exists. |
| `delete <key>` | Delete the element with the given key. |
| `search <key>` | Find the element with the given key and print its value. |
| `print` | Print the entire table. |
| `import <filename>` | Load the table state from a binary file. |
| `export <filename>` | Save the table state to a binary file. |
| `help` | Show available commands. |
| `exit` | Quit the program. |

Example session:

```text
> insert 10 100
Element inserted.
> insert 20 200
Element inserted.
> search 10
Key: 10, Value: 100
> print
Key: 10, Value: 100
Key: 20, Value: 200
> delete 10
Element deleted.
> export table.bin
Table exported to table.bin.
> exit
```

## Implementation notes

### Hashing

- The table uses **double hashing** to resolve collisions.
- The primary hash function and the step size are derived from the key.
- The step size is never zero.

### Resizing

- When the table is full (`csize == msize`), it automatically grows.
- The new size is chosen as the next prime number greater than twice the current size (or a similar strategy).
- All elements are rehashed into the new vector.

### Memory management

- Each `KeySpace` slot holds a pointer to dynamically allocated `InfoType` (unsigned integer).
- `table_destroy` frees all allocated values and the vector itself.
- The program does not use global variables.
- The project is intended to be clean under Valgrind and sanitizers.

### Error handling

- Functions return error codes (e.g., `0` on success, negative values on failure).
- The interactive program prints human-readable error messages and continues execution.

## Binary file format

The binary file stores the table in a simple format:

1. `msize` (IndexType)
2. `csize` (IndexType)
3. For each slot:
   - `busy` (BusyType)
   - `key` (KeyType)
   - `value` (InfoType) — only if the slot is occupied

This format allows exact restoration of the table state.

## License

MIT License. See `LICENSE` if included.
