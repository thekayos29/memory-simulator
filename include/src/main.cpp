#include <iostream>
#include <string>
#include "allocator/allocator.cpp"
#include "cache/cache.cpp"

int main() {
    MemoryManager* mem = nullptr;
    CacheSim* L1 = nullptr;
    CacheSim* L2 = nullptr;
    
    std::string cmd;
    std::string strategy = "first";

    std::cout << "________________________________________" << std::endl;
    std::cout << "   MEMORY MANAGEMENT SIMULATOR ACTIVE       " << std::endl;
    std::cout << "________________________________________" << std::endl;

    while (true) {
        std::cout << "[" << strategy << "] > ";
        if (!(std::cin >> cmd)) break;

        if (cmd == "exit") break;

        else if (cmd == "init_mem") {
            size_t size; std::cin >> size;
            if (mem) delete mem;
            mem = new MemoryManager(size);
            std::cout << "MEMORY INITIALISED" << std::endl;
        }

        else if (cmd == "init_cache") {
            std::string level; int s, bs, assoc;
            std::cin >> level >> s >> bs >> assoc;
            if (level == "L1") {
                if (L1) delete L1;
                L1 = new CacheSim(s, bs, assoc);
            } else if (level == "L2") {
                if (L2) delete L2;
                L2 = new CacheSim(s, bs, assoc);
            }
            std::cout << "System: " << level << " configured." << std::endl;
        }

        else if (cmd == "set_strategy") {
            std::string choice; std::cin >> choice;
            if (choice == "first" || choice == "best" || choice == "worst") {
                strategy = choice;
                std::cout << "System: Strategy changed to " << choice << " fit." << std::endl;
            } else {
                std::cout << "Error: Use 'first', 'best', or 'worst'." << std::endl;
            }
        }

        else if (cmd == "malloc") {
            size_t size; std::cin >> size;
            if (!mem) { std::cout << "Error: Init memory first." << std::endl; continue; }

            int id = -1;
            if (strategy == "first") id = mem->malloc_first(size);
            else if (strategy == "best") id = mem->malloc_best(size);
            else if (strategy == "worst") id = mem->malloc_worst(size);

            if (id != -1) {
                std::cout << "Success: Allocated ID " << id << std::endl;
                
                // Simulate address access (using ID * 64 as a symbolic address)
                size_t addr = id * 64;
                if (L1) {
                    if (!L1->access(addr)) { // L1 Miss
                        if (L2) L2->access(addr); // Check L2
                    }
                }
            } else {
                std::cout << "Failure: Not enough space." << std::endl;
            }
        }

        else if (cmd == "free") {
            int id; std::cin >> id;
            if (mem) mem->free_mem(id);
        }

        else if (cmd == "dump") {
            if (mem) mem->dump();
        }

        else if (cmd == "stats") {
            if (mem) mem->print_stats();
            if (L1) { std::cout << "L1 Cache: "; L1->stats(); }
            if (L2) { std::cout << "L2 Cache: "; L2->stats(); }
        }
    }

    // Cleanup
    if (mem) delete mem;
    if (L1) delete L1;
    if (L2) delete L2;

    return 0;
}