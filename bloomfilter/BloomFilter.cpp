#include "BloomFilter.h"

BloomFilter::BloomFilter(uint64_t m, std::vector<std::unique_ptr<HashFunction>>&& hashFunctions) {
    table.resize(m);
    this->hashFunctions = std::move(hashFunctions);
}

void BloomFilter::add(uint64_t x) {
    for(const auto& h : hashFunctions) { 
        table[h->hash(x)] = true;
    }
}

bool BloomFilter::contains(uint64_t x) {
    for(const auto& h : hashFunctions) {
        if(!table[h->hash(x)]) return false;
    }
    return true;
}