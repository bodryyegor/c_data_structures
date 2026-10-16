# Clinic Queue Simulator

A small C project that implements a double-ended queue (deque) and uses it to simulate a clinic waiting room. Patients arrive over time, and the queue is updated according to a simple rule:

- Regular patients join the end of the queue.
- Patients who need to “just ask a question” join the front of the queue.

The simulator prints the current queue whenever its state changes.

## Features

- Two interchangeable deque backends:
  - `deque_vector.c` — circular buffer / dynamic array.
  - `deque_list.c` — doubly linked list.
- Common public API in `deque.h`.
- Opaque `Deque` type: the application does not depend on the internal representation.
- Event-driven simulation in `main.c`.
- No global variables.
- The list backend grows without a fixed limit.
- The vector backend uses a fixed-capacity buffer allocated once and fails gracefully when full.
- Clean memory management; suitable for Valgrind and AddressSanitizer.

## Project layout

```text
.
├── deque.h          # Public deque interface
├── deque_list.c     # Linked-list implementation
├── deque_vector.c   # Vector implementation
└── main.c           # Clinic queue simulator
```

## Building

No Makefile is included. Compile the project directly with your C compiler. Choose exactly one deque implementation.

### Vector backend

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c deque_vector.c -o clinic_queue
```

### List backend

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -O2 main.c deque_list.c -o clinic_queue
```

### Debug build

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined main.c deque_vector.c -o clinic_queue_asan
```

## Running

```bash
./clinic_queue < input.txt
```

Or interactively:

```bash
./clinic_queue
```

## Input format

Each patient record has the form:

```text
[*]id/ta/ts
```

where:

- `id` — patient identifier (string without spaces),
- `ta` — arrival time (natural number),
- `ts` — service time (natural number),
- optional leading `*` — the patient needs to “just ask a question” and joins the front of the queue.

Records are separated by whitespace. Multiple records may appear on one line. Input ends at EOF.

Example:

```text
a/1/20 b/1/15 c/2/10 *d/5/8 *e/6/5 f/6/9
```

## Output

Whenever the queue state changes, the program prints the queue from front to back. Patient identifiers are separated by spaces.

Example output fragment:

```text
a b
a b c
d a b c
e d a b c
e d a b c f
```

## Implementation notes

### Vector backend (`deque_vector.c`)

- Uses a fixed-capacity circular buffer.
- The buffer is allocated once when the deque is created.
- `push_front` and `push_back` return an error when the deque is full.
- All operations are O(1).

### List backend (`deque_list.c`)

- Uses a doubly linked list with head and tail pointers.
- Supports an arbitrary number of elements.
- `push_front`, `push_back`, `pop_front`, and `pop_back` are O(1).
- Each node is allocated and freed dynamically.

### Simulator (`main.c`)

- Reads all patient records from standard input.
- Processes events in chronological order.
- Maintains the doctor’s availability and the patient queue.
- Uses the deque as the waiting line:
  - regular patient → `deque_push_back`,
  - “just ask” patient → `deque_push_front`.
- Prints the queue after every change.

## Memory management

- The deque itself does not free patient data.
- `main.c` allocates patient records and releases them after service.
- `deque_destroy` releases only the internal storage of the deque.
- The project is intended to be clean under Valgrind and sanitizers.

## License

MIT License. See `LICENSE` if included.
