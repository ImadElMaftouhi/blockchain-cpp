#include "blockchain_pos.h"
#include "blockchain_pow.h" // Added to resolve undefined BlockchainPow
#include "transaction.h"
#include "validator.h"
#include "utils.h"
#include <vector>
#include <iostream>
#include <cassert>

int main() {
    std::vector<Validator> validators = {{"V1", 50}, {"V2", 30}, {"V3", 20}};
    BlockchainPos posChain(2, validators);
    std::vector<Transaction> txs = {Transaction("T1", "Alice", "Bob", 50.0), Transaction("T2", "Bob", "Charlie", 30.0)};
    std::vector<std::string> txStrings;
    for (const auto& tx : txs) txStrings.push_back(tx.toString());
    posChain.addBlock(txStrings);
    posChain.addBlock(txStrings);
    posChain.displayChain();
    assert(posChain.isChainValid());
    std::cout << "PoS Test Passed: Chain is valid.\n";

    BlockchainPow powChain(2);
    powChain.addBlock(txStrings);
    powChain.setDifficulty(3);
    powChain.addBlock(txStrings);
    long long powTime = measureTime([&]() { powChain.addBlock(txStrings); });
    long long posTime = measureTime([&]() { posChain.addBlock(txStrings); });
    std::cout << "PoW Time: " << powTime << " ms, PoS Time: " << posTime << " ms\n";
    std::cout << "PoW vs PoS Test Passed: Times measured.\n";
    return 0;
}