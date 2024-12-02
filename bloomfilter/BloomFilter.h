#pragma once

#include "HashFunctions.h"

#include <cstdint>
#include <memory>
#include <vector>
 
enum class HashType {
    PRIME_MOD,
    MERSENNE_TWIST
};

class BloomFilter {
    public:
        BloomFilter(uint64_t m, std::vector<std::unique_ptr<HashFunction>>&& hashFunctions);

        void add(uint64_t x);
        bool contains(uint64_t x);

    private:
        std::vector<bool> table;
        std::vector<std::unique_ptr<HashFunction>> hashFunctions;
};