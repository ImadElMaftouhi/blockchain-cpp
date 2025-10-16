#include "blockchain_pow.h"
#include "blockchain_pos.h"
#include "transaction.h"
#include "validator.h"
#include "utils.h"
#include <vector>
#include <iostream>
#include <cassert>

int main() {
    // Partie 1 - Structure des blocs et de la chaîne
    std::vector<Transaction> txs = {Transaction("T1", "Alice", "Bob", 50.0), Transaction("T2", "Bob", "Charlie", 30.0)};
    std::vector<std::string> txStrings;
    for (const auto& tx : txs) txStrings.push_back(tx.toString());
    assert(txs[0].validate());
    std::cout << "Partie 1 Test Passed: Transaction validation works.\n";

    // Partie 2 - Proof of Work
    BlockchainPow powChain(2);
    powChain.addBlock(txStrings);
    powChain.setDifficulty(2);
    powChain.addBlock(txStrings);
    powChain.setDifficulty(3);
    powChain.addBlock(txStrings);
    assert(powChain.isChainValid());
    std::cout << "Partie 2 Test Passed: PoW chain is valid with varying difficulties.\n";

    // Partie 3 - Proof of Stake
    std::vector<Validator> validators = {{"V1", 50}, {"V2", 30}, {"V3", 20}};
    BlockchainPos posChain(2, validators);
    posChain.addBlock(txStrings);
    posChain.addBlock(txStrings);
    assert(posChain.isChainValid());
    std::cout << "Partie 3 Test Passed: PoS chain is valid.\n";

    // Partie 4 - Analyse comparative
    long long powTime = measureTime([&]() { powChain.addBlock(txStrings); });
    long long posTime = measureTime([&]() { posChain.addBlock(txStrings); });
    std::cout << "Partie 4 Test Passed: PoW Time: " << powTime << " ms, PoS Time: " << posTime << " ms\n";
    std::cout << "Rapidity: " << (powTime < posTime ? "PoW" : "PoS") << " is faster\n";
    std::cout << "Resource Consumption: PoW uses more CPU due to mining\n";
    std::cout << "Ease of Implementation: PoS is simpler\n";

    std::cout << "Exercise 4 Complete Test Passed.\n";
    return 0;
}