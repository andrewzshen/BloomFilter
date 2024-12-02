#include "BloomFilter.h"
#include "HashFunctions.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <set>
#include <fstream>

constexpr uint64_t universeSize = 1'000'000'000'000;
constexpr uint64_t prime = (1UL << 61) - 1; // 2^61 - 1
constexpr uint64_t seed = 42069;
constexpr uint64_t numQueries = 10'000;
constexpr uint64_t numTrials = 10;
constexpr uint64_t numElements = 10'000;

constexpr std::array<uint64_t, 2> cVals = {5, 10};
constexpr std::array<uint64_t, 4> kVals = {2, 4, 6, 8};

double theoreticalFalsePositiveRate(uint64_t m, uint64_t n, uint64_t k) {
    double exponent = -static_cast<double>(k * n) / m;
    return std::pow(1 - std::exp(exponent), k);
}

double falsePositiveRateExperiment(uint64_t c, uint64_t numHashFunctions, HashType hashType) {
    uint64_t tableSize = c * numElements;
    double theoreticalRate = theoreticalFalsePositiveRate(tableSize, numElements, numHashFunctions);
    double totalEmpiricalRate = 0.0;

    for(size_t i = 0; i < numTrials; i++) {
        std::vector<std::unique_ptr<HashFunction>> hashFunctions;
        
        if(hashType == HashType::PRIME_MOD) {
            for(int j = 0; j < numHashFunctions; j++) {
                hashFunctions.emplace_back(std::make_unique<PrimeModHashFunction>(prime, tableSize));
            }
        } else {
            for(int j = 0; j < numHashFunctions; j++) {
                hashFunctions.emplace_back(std::make_unique<MersenneTwistHashFunction>(prime, tableSize));
            }
        }
        
        BloomFilter bloomFilter(tableSize, std::move(hashFunctions));

        std::set<uint64_t> inserted;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, tableSize);

        while(inserted.size() < numElements) {
            inserted.insert(dist(gen));
        }

        for(int x : inserted) {
            bloomFilter.add(x);
        }

        int falsePositives = 0;
        for (size_t j = 0; j < numQueries; j++) {
            int query = dist(gen);
            while(inserted.find(query) != inserted.end()) {
                query = dist(gen);
            } 

            if(bloomFilter.contains(query)) {
                falsePositives++;
            }
        }

        double empiricalRate = static_cast<double>(falsePositives) / numQueries;
        totalEmpiricalRate += empiricalRate;
    }

    return totalEmpiricalRate / numTrials;
}

int main() {
    // std::ofstream outfile("../data/hashdata.txt");

    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<uint64_t> dist(0, universeSize - 1);

    // PrimeModHashFunction pm(prime, tableSize);
    // MersenneTwistHashFunction mt(seed, tableSize);

    // for(size_t i = 0; i < 10000; i++) {
    //     uint64_t x = dist(gen);
    //     uint64_t y1 = pm.hash(x);
    //     uint64_t y2 = mt.hash(x);

    //     outfile << x << " " << y1 << " " << y2 << "\n";
    // }

    // outfile.close();

    std::ofstream outfile("../data/bloomfilter.txt");

    for(uint64_t c : cVals) {
        for(uint64_t k : kVals) {
            outfile << falsePositiveRateExperiment(c, k, HashType::PRIME_MOD);
            outfile << falsePositiveRateExperiment(c, k, HashType::MERSENNE_TWIST);
        }
    }

    outfile.close();

    return 0;
}