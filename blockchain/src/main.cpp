#include <iostream>
#include "blockchain.h"
#include "pow.h"
#include "pos.h"
#include <chrono>

int main() {
    std::cout << "=== Exercice 4: Mini-Blockchain Demonstration (October 16, 2025, 09:36 PM +01) ===" << std::endl;

    // Exercice 1: Merkle Tree Demo
    std::cout << "\n--- Exercice 1: Merkle Tree ---" << std::endl;
    std::vector<std::string> transactions = {
        "Alice sends 50 BTC to Bob",
        "Bob sends 30 BTC to Charlie"
    };
    MerkleTree mt(transactions);
    mt.printTree();
    std::cout << "Root Hash: " << mt.getRootHash() << std::endl;
    std::cout << "Verify 'Alice sends 50 BTC to Bob': " << (mt.verifyTransaction("Alice sends 50 BTC to Bob") ? "Yes" : "No") << std::endl;

    // Exercice 2: Proof of Work Demo
    std::cout << "\n--- Exercice 2: Proof of Work ---" << std::endl;
    Blockchain powChain(2);
    powChain.addBlock("Alice sends 50 BTC to Bob");
    powChain.addBlock("Bob sends 30 BTC to Charlie");
    std::cout << "PoW Chain Valid: " << (powChain.isChainValid() ? "Yes" : "No") << std::endl;
    powChain.displayChain();

    // Exercice 3: Proof of Stake Demo
    std::cout << "\n--- Exercice 3: Proof of Stake ---" << std::endl;
    std::vector<Validator> validators = {{"V1", 50}, {"V2", 30}, {"V3", 20}};
    PoSBlockchain posChain(validators);
    posChain.addBlock("Alice sends 50 BTC to Bob");
    posChain.addBlock("Bob sends 30 BTC to Charlie");
    std::cout << "PoS Chain Valid: " << (posChain.isChainValid() ? "Yes" : "No") << std::endl;
    posChain.displayChain();

    // Exercice 4: Mini-Blockchain
    std::cout << "\n--- Exercice 4: Mini-Blockchain ---" << std::endl;
    MiniBlockchain miniChain(2);
    std::vector<Transaction> txs = {Transaction("tx1", "Alice", "Bob", 50.0), Transaction("tx2", "Bob", "Charlie", 30.0)};
    miniChain.addBlock(txs);
    miniChain.addBlock({Transaction("tx3", "Charlie", "David", 20.0)});
    std::cout << "Mini Blockchain Valid: " << (miniChain.isValid() ? "Yes" : "No") << std::endl;
    miniChain.displayChain();

    // Partie 4: Analysis
    std::cout << "\n=== Partie 4: Comparative Analysis ===" << std::endl;
    std::vector<long long> powTimes, posTimes, miniTimes;
    const int blocksToAdd = 5;
    std::vector<Transaction> sampleTx = {Transaction("tx4", "David", "Eve", 10.0)};

    std::cout << "Adding " << blocksToAdd << " blocks to each chain...\n";
    for (int i = 0; i < blocksToAdd; i++) {
        auto startPow = std::chrono::high_resolution_clock::now();
        powChain.addBlock("Transaction " + std::to_string(i));
        auto endPow = std::chrono::high_resolution_clock::now();
        powTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endPow - startPow).count());

        auto startPos = std::chrono::high_resolution_clock::now();
        posChain.addBlock("Transaction " + std::to_string(i));
        auto endPos = std::chrono::high_resolution_clock::now();
        posTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endPos - startPos).count());

        auto startMini = std::chrono::high_resolution_clock::now();
        miniChain.addBlock({Transaction("tx" + std::to_string(i + 5), "Eve", "Frank", 5.0 + i)});
        auto endMini = std::chrono::high_resolution_clock::now();
        miniTimes.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(endMini - startMini).count());
    }

    long long totalPowTime = 0, totalPosTime = 0, totalMiniTime = 0;
    for (int i = 0; i < blocksToAdd; i++) {
        totalPowTime += powTimes[i];
        totalPosTime += posTimes[i];
        totalMiniTime += miniTimes[i];
    }
    double avgPowTime = totalPowTime / (double)blocksToAdd;
    double avgPosTime = totalPosTime / (double)blocksToAdd;
    double avgMiniTime = totalMiniTime / (double)blocksToAdd;

    std::cout << "Average PoW Block Addition Time: " << avgPowTime << " ms" << std::endl;
    std::cout << "Average PoS Block Addition Time: " << avgPosTime << " ms" << std::endl;
    std::cout << "Average Mini Blockchain Addition Time: " << avgMiniTime << " ms" << std::endl;
    std::cout << "PoW is resource-intensive due to mining; PoS is faster but depends on validator stakes; Mini Blockchain balances both with Merkle Trees." << std::endl;
    std::cout << "Ease of Implementation: PoS simplest, PoW complex due to mining, Mini Blockchain moderate due to Merkle integration." << std::endl;

    return 0;
}