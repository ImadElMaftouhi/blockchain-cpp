#include "utils.h"
#include "merkle_tree.h"
#include "transaction.h"
#include "block_pow.h"
#include "block_pos.h"
#include "blockchain_pow.h"
#include "blockchain_pos.h"
#include "validator.h"
#include <iostream>
#include <vector>

int main() {
    // Exercise 1: Merkle Tree Demo
    std::cout << "=== Exercise 1: Merkle Tree Demo ===\n";
    std::vector<std::string> transactions1 = {"Tx1: Alice->Bob:50", "Tx2: Bob->Charlie:30"};
    MerkleTree merkleTree(transactions1);
    merkleTree.printTree();
    std::cout << "Root Hash: " << merkleTree.getRootHash() << "\n\n";

    // Exercise 2: Proof of Work Demo
    std::cout << "=== Exercise 2: Proof of Work Demo ===\n";
    BlockchainPow powChain(2);
    powChain.addBlock(transactions1);
    powChain.setDifficulty(3);
    powChain.addBlock(transactions1);
    powChain.displayChain();
    std::cout << "PoW Chain Valid: " << (powChain.isChainValid() ? "Yes" : "No") << "\n\n";

    // Exercise 3: Proof of Stake Demo
    std::cout << "=== Exercise 3: Proof of Stake Demo ===\n";
    std::vector<Validator> validators = {{"V1", 50}, {"V2", 30}, {"V3", 20}};
    BlockchainPos posChain(2, validators);
    posChain.addBlock(transactions1);
    posChain.addBlock(transactions1);
    posChain.displayChain();
    std::cout << "PoS Chain Valid: " << (posChain.isChainValid() ? "Yes" : "No") << "\n\n";

    // Exercise 4: Complete Blockchain Manipulation
    std::cout << "=== Exercise 4: Complete Blockchain Demo ===\n";

    // Partie 1 - Structure des blocs et de la chaîne
    std::vector<Transaction> txs = {
        Transaction("T1", "Alice", "Bob", 50.0),
        Transaction("T2", "Bob", "Charlie", 30.0)
    };
    std::vector<std::string> txStrings;
    for (const auto& tx : txs) {
        txStrings.push_back(tx.toString());
    }

    // Partie 2 - Proof of Work
    BlockchainPow powChainFull(2);
    powChainFull.addBlock(txStrings);
    powChainFull.setDifficulty(2);
    powChainFull.addBlock(txStrings);
    powChainFull.setDifficulty(3);
    powChainFull.addBlock(txStrings);
    std::cout << "PoW Chain (Difficulties 2, 3):\n";
    powChainFull.displayChain();
    long long powTime = measureTime([&]() { powChainFull.addBlock(txStrings); });
    std::cout << "PoW Block Addition Time (Difficulty 3): " << powTime << " ms\n";

    // Partie 3 - Proof of Stake
    BlockchainPos posChainFull(2, validators);
    posChainFull.addBlock(txStrings);
    posChainFull.addBlock(txStrings);
    std::cout << "PoS Chain:\n";
    posChainFull.displayChain();
    long long posTime = measureTime([&]() { posChainFull.addBlock(txStrings); });
    std::cout << "PoS Block Addition Time: " << posTime << " ms\n";

    // Partie 4 - Analyse comparative
    std::cout << "\n=== Comparative Analysis ===\n";
    std::cout << "PoW Time: " << powTime << " ms, PoS Time: " << posTime << " ms\n";
    std::cout << "Rapidity: " << (powTime < posTime ? "PoW is faster" : "PoS is faster") << "\n";
    std::cout << "Resource Consumption: PoW requires more CPU due to mining, PoS is lighter\n";
    std::cout << "Ease of Implementation: PoS is simpler due to no mining loop\n";

    std::cout << "\n=== All Exercises Completed ===\n";
    return 0;
}