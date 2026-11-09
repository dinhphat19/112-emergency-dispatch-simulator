# 112 Emergency Call Dispatch Simulator

A low-level C simulation of Romania's 112 emergency response workflow (STS). This project manages incident prioritization, unit allocation, and intervention dispatching using complex data structures and strict manual memory management.

## Key Features & Data Structures
- **Incident & Intervention Tracking:** Utilizes **circular doubly linked lists with sentinels** to achieve O(1) insertion and deletion times for active incidents and dispatched units.
- **Pointer-Based Priority Queues:** Implements multi-level queuing (`high`, `medium`, `low`) for incoming incidents. To optimize memory, the queues do not duplicate data but store pointers directly to the linked list nodes.
- **Dispatch & Rollback Mechanism:** Features an automated `DISPATCH` engine that pairs the highest-priority incident with available units, and a Stack-based `UNDO_LAST_DISPATCH` mechanism to safely revert ongoing interventions and restore unit availability.
- **Memory Safety:** Strictly adheres to dynamic memory allocation principles with zero memory leaks, verified via **Valgrind**.

## Tech Stack
- **Language:** C
- **Tools:** GCC, Make, Valgrind, Linux (WSL)
- **Concepts:** Circular Doubly Linked Lists, Sentinels, Priority Queues, Stacks, Pointers.

## Build & Run
To compile the project, run:
make build