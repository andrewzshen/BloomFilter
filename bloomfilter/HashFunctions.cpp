#include "HashFunctions.h"

#include <random>

PrimeModHashFunction::PrimeModHashFunction(uint64_t p, uint64_t m) {
    this->p = p;
    this->m = m;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<uint64_t> distA(1, p - 1);
    std::uniform_int_distribution<uint64_t> distB(0, p - 1);

    a = distA(gen);
    b = distB(gen);
}

uint64_t PrimeModHashFunction::hash(uint64_t x) const {
    return ((a * x + b) % p) % m;
}

MersenneTwistHashFunction::MersenneTwistHashFunction(uint64_t s, uint64_t m) {
    this->s = s;
    this->m = m;
}

uint64_t MersenneTwistHashFunction::hash(uint64_t x) const {
    std::mt19937 gen(s + x);
    std::uniform_int_distribution<uint64_t> dist(0, m - 1);
    return dist(gen);
}