#include <iostream>
#include <vector>
#include <list>

class CacheSim {
private:
    int size, block_size, associativity;
    std::vector<std::list<size_t>> sets;
    int hits, misses;

public:
    CacheSim(int s, int bs, int assoc) : size(s), block_size(bs), associativity(assoc) {
        // calculation of sets
        int num_sets = (size / (block_size * associativity));
        if (num_sets == 0) num_sets = 1; // Safeguarding for small cache
        sets.resize(num_sets);
        hits = 0;
        misses = 0;
    }

    bool access(size_t address) {
        size_t block_addr = address / block_size;
        size_t set_idx = block_addr % sets.size();
        auto& set = sets[set_idx];

        // equality for tag
        for (size_t tag : set) {
            if (tag == block_addr) {
                hits++;
                return true; // Hit
            }
        }

        // fifo repplacement
        misses++;
        if (set.size() >= (size_t)associativity) {
            set.pop_front(); // Remove the oldest element
        }
        set.push_back(block_addr);
        return false; // Miss
    }

    void stats() {
        double ratio = (hits + misses == 0) ? 0 : (double)hits / (hits + misses);
        std::cout << "Hits: " << hits << " | Misses: " << misses 
                  << " | Hit Ratio: " << ratio << std::endl;
    }
};