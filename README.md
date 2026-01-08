# Design and Implementation of a Memory Management Simulator

C++ code for memory management simulator
---

## 📂 Project Structure

| Component | File Path | Responsibility |
| :--- | :--- | :--- |
| **Main Controller** | `main.cpp` | CLI handling and system orchestration. |
| **Allocator** | `allocator/allocator.cpp` | Doubly linked-list management (Splitting/Merging). |
| **Cache Sim** | `cache/cache.cpp` | Cache initialisation and its functioning |

---

## 🛠 Command Reference

### 1. Initialization Commands
| Command | Arguments | Description |
| :--- | :--- | :--- |
| `init_mem` | `<size>` | Initializes total physical RAM (e.g., `init_mem 1024`). |
| `init_cache` | `<L1\|L2> <size> <line> <assoc>` | Configures cache level parameters. |
| `set_strategy` | `<first\|best\|worst>` | Swaps the allocation algorithm (Default: First Fit). |

### 2. Operational & Debugging Commands
| Command | Arguments | Description |
| :--- | :--- | :--- |
| `malloc` | `<size>` | Allocates memory and triggers a cache access trace. |
| `free` | `<id>` | Releases a block and merges adjacent free segments. |
| `dump` | `N/A` | Prints a visual hex-map of the physical memory. |
| `stats` | `N/A` | Displays fragmentation and cache hit/miss statistics. |

---

## ⚙️ System Logic & Algorithms

### Allocation Strategies
| Strategy | Logic | Use Case |
| :--- | :--- | :--- |
| **First Fit** | Scans list and takes the first available hole. | High speed, low overhead. |
| **Best Fit** | Scans all holes to find the smallest suitable one. | Minimizes wasted space per block. |
| **Worst Fit** | Scans all holes to find the largest available one. | Prevents creation of many tiny holes. |



### Cache Replacement Policy
| Policy | Logic | Implementation |
| :--- | :--- | :--- |
| **FIFO** | First-In, First-Out | Oldest block in the set is evicted on a miss. |
| **Associativity** | Set-Associative | Balances search speed and collision reduction. |



---

## ⚠️ Troubleshooting & Errors

| Error Message | Meaning | Fix |
| :--- | :--- | :--- |
| `Error: Init memory first` | System not initialized. | Run `init_mem` before `malloc`. |
| `Failure: Not enough space` | No contiguous block fits the size. | Run `free` on unused IDs or check `dump`. |
| `Error: ID not found` | ID is invalid or already free. | Verify active IDs using the `dump` command. |

---

## 📊 Performance Metrics

| Metric | Description | Formula |
| :--- | :--- | :--- |
| **External Fragmentation** | Measures unusable gaps in RAM. | $1.0 - (\text{Max\_Free\_Block} / \text{Total\_Free\_Memory})$ |
| **Cache Hit Ratio** | Measures cache efficiency. | $\text{Hits} / (\text{Hits} + \text{Misses})$ |



---

## Example Script
After compiling the script, run the program.
1. `init_mem 1024`
2. `init_cache L1 64 16 1`
3. `malloc 100`  *(Should result in L1 Miss)*
4. `free 1`
5. `dump` *(Check: Memory map should show block 1 is now FREE)*
6. `stats` *( Fragmentation should be 0% if memory is contiguous)*
7. `set_strategy` (best worst or first)

---
**Author:** Swarnam Nitnaware 

**Enrolment No:** 24123028
 
