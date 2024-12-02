#pragma once

#include <cstdint>

class HashFunction {
    public:
        virtual ~HashFunction() = default;
        virtual uint64_t hash(uint64_t val) const = 0;
};

class PrimeModHashFunction : public HashFunction {
    public:
        PrimeModHashFunction(uint64_t p, uint64_t m);
        
        uint64_t hash(uint64_t x) const;

    private:
        uint64_t a, b;
        uint64_t p;
        uint64_t m;
};

class MersenneTwistHashFunction : public HashFunction {
    public:
        MersenneTwistHashFunction(uint64_t s, uint64_t m);
        
        uint64_t hash(uint64_t x) const;
    
    private:
        uint64_t s;
        uint64_t m;
};
