#include "blockchain_pow.h"
#include "transaction.h"
#include <vector>
#include <iostream>
#include <cassert>

int main() {
    BlockchainPow powChain(2);
    std::vector<Transaction> txs = {Transaction("T1", "Alice", "Bob", 50.0), Transaction("T2", "Bob", "Charlie", 30.0)};
    std::vector<std::string> txStrings;
    for (const auto& tx : txs) txStrings.push_back(tx.toString());
    powChain.addBlock(txStrings);
    powChain.setDifficulty(3);
    powChain.addBlock(txStrings);
    powChain.displayChain();
    assert(powChain.isChainValid());
    std::cout << "PoW Test Passed: Chain is valid with different difficulties.\n";
    return 0;
}