#include "blockchain_pow.h"
#include "utils.h"
#include "pow.h"
#include <iostream>
#include <chrono>

BlockchainPow::BlockchainPow(int diff) : Blockchain(diff) {
    std::string genesisData = "Genesis Block";
    int nonce = 0;
    std::string genesisHash = ProofOfWork::mineBlock(genesisData, "0", difficulty, nonce);
    chain.push_back(new BlockPow(0, "0", genesisHash, genesisData, nonce, difficulty));
}

BlockchainPow::~BlockchainPow() {
    for (auto* block : chain) {
        delete block;
    }
}

void BlockchainPow::addBlock(const std::vector<std::string>& transactions) {
    MerkleTree merkleTree(transactions);
    std::string merkleRoot = merkleTree.getRootHash();
    std::string prevHash = getLatestHash();
    int nonce = 0;
    auto start = std::chrono::high_resolution_clock::now();
    std::string newHash = ProofOfWork::mineBlock(merkleRoot, prevHash, difficulty, nonce);
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = measureTime([&]() {});
    chain.push_back(new BlockPow(chain.size(), prevHash, newHash, merkleRoot, nonce, difficulty));
    std::cout << "Block #" << chain.size() - 1 << " mined in " << duration << " ms" << std::endl;
}

bool BlockchainPow::isChainValid() const {
    for (size_t i = 1; i < chain.size(); i++) {
        if (!ProofOfWork::verifyBlock(chain[i]->getData(), chain[i]->getPreviousHash(), chain[i]->getHash(), chain[i]->getDifficulty(), chain[i]->getNonce())) {
            return false;
        }
        if (chain[i]->getPreviousHash() != chain[i-1]->getHash()) return false;
    }
    return true;
}

void BlockchainPow::displayChain() const {
    for (const auto* block : chain) {
        block->display();
    }
}

void BlockchainPow::setDifficulty(int diff) {
    difficulty = diff;
}

std::string BlockchainPow::getLatestHash() const {
    return chain.empty() ? "0" : chain.back()->getHash();
}