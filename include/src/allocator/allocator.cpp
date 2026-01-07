#include <iostream>
#include <string>
#include <iomanip>
//Initializes a memory segment with ID, size, address, and status.
struct Block {
    int id;
    size_t size;
    size_t address;
    bool is_free;
    Block* next;
    Block* prev;

    Block(int i, size_t s, size_t addr, bool free) 
        : id(i), size(s), address(addr), is_free(free), next(nullptr), prev(nullptr) {}
};

// memory manager class
class MemoryManager {
private:
    Block* head;
    size_t total_size;
    int next_id;

    void split_block(Block* b, size_t size) {
        if (b->size > size) {
            Block* new_free = new Block(0, b->size - size, b->address + size, true);
            new_free->next = b->next;
            if (b->next) b->next->prev = new_free;
            b->next = new_free;
            new_free->prev = b;
            b->size = size;
        }
        b->is_free = false;
        b->id = next_id++;
    }

public:
    MemoryManager(size_t size) : total_size(size), next_id(1) {
        head = new Block(0, size, 0, true);
    }

    // Strategies: First, Best, Worst Fit
    int malloc_first(size_t size) {
        Block* curr = head;
        while (curr) {
            if (curr->is_free && curr->size >= size) {
                split_block(curr, size);
                return curr->id;
            }
            curr = curr->next;
        }
        return -1;
    }

    int malloc_best(size_t size) {
        Block* best = nullptr;
        Block* curr = head;
        while (curr) {
            if (curr->is_free && curr->size >= size) {
                if (!best || curr->size < best->size) best = curr;
            }
            curr = curr->next;
        }
        if (best) { split_block(best, size); return best->id; }
        return -1;
    }

    int malloc_worst(size_t size) {
        Block* worst = nullptr;
        Block* curr = head;
        while (curr) {
            if (curr->is_free && curr->size >= size) {
                if (!worst || curr->size > worst->size) worst = curr;
            }
            curr = curr->next;
        }
        if (worst) { split_block(worst, size); return worst->id; }
        return -1;
    }

    void free_mem(int id) {
        Block* curr = head;
        while (curr) {
            if (!curr->is_free && curr->id == id) {
                curr->is_free = true;
                // Coalescing (Merging)
                if (curr->next && curr->next->is_free) {
                    Block* temp = curr->next;
                    curr->size += temp->size;
                    curr->next = temp->next;
                    if (temp->next) temp->next->prev = curr;
                    delete temp;
                }
                if (curr->prev && curr->prev->is_free) {
                    Block* p = curr->prev;
                    p->size += curr->size;
                    p->next = curr->next;
                    if (curr->next) curr->next->prev = p;
                    delete curr;
                }
                return;
            }
            curr = curr->next;
        }
    }

    void dump() {
        Block* curr = head;
        std::cout << "\n PHYSICAL MEMORY MAP " << std::endl;
        while (curr) {
            std::cout << "[0x" << std::hex << std::setw(4) << std::setfill('0') << curr->address 
                      << "] /////// " << std::dec << std::setfill(' ') << std::setw(10) 
                      << (curr->is_free ? "FREE" : "USED (ID:" + std::to_string(curr->id) + ")")
                      << " /////// Size: " << curr->size << " bytes" << std::endl;
            curr = curr->next;
        }
    }

    void print_stats() {
        size_t total_free = 0, max_free = 0, used = 0;
        Block* curr = head;
        while (curr) {
            if (curr->is_free) {
                total_free += curr->size;
                if (curr->size > max_free) max_free = curr->size;
            } else used += curr->size;
            curr = curr->next;
        }
        double frag = (total_free == 0) ? 0 : (1.0 - (double)max_free / total_free) * 100;
        std::cout << "Used: " << used << " | Free: " << total_free << " | Ext. Frag: " << frag << "%\n";
    }
};