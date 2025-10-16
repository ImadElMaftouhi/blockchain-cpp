#include "blockchain.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <merkle_tree.h>

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

MiniBlock::MiniBlock(int idx, const std::string& prevHash, const std::vector<Transaction>& txs)
    : Block(idx, "", prevHash), transactions(txs), merkleRoot("") {
    std::vector<std::string> txStrings;
    for (const auto& tx : transactions) {
        txStrings.push_back(tx.toString());
    }
    merkleRoot = MerkleTree(txStrings).getRootHash();
    timestamp = std::time(nullptr);
    nonce = 0;
    hash = calculateHash();
    hash = calculateHash();
}

std::string MiniBlock::calculateHash() const {
    std::stringstream ss;
    ss << index << timestamp << previousHash << merkleRoot << nonce;
    return sha256(ss.str());
}

void MiniBlock::mineBlock(int difficulty) {
    this->difficulty = difficulty;
    std::string target(difficulty, '0');
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        hash = calculateHash();
    }
}

void MiniBlock::displayBlock() const {
    std::cout << "\n--- Mini Block #" << index << " ---" << std::endl;
    std::cout << "Timestamp: " << ctime(&timestamp);
    std::cout << "Merkle Root: " << merkleRoot.substr(0, 16) << "..." << std::endl;
    std::cout << "Previous Hash: " << previousHash.substr(0, 16) << "..." << std::endl;
    std::cout << "Hash: " << hash.substr(0, 16) << "..." << std::endl;
    std::cout << "Nonce: " << nonce << std::endl;
    std::cout << "Transactions: ";
    for (const auto& tx : transactions) {
        std::cout << tx.id << " ";
    }
    std::cout << std::endl;
}

MiniBlockchain::MiniBlockchain(int diff) : difficulty(diff) {
    std::vector<Transaction> genesisTxs;
    chain.emplace_back(0, "0", genesisTxs);
}

void MiniBlockchain::addBlock(const std::vector<Transaction>& txs) {
    auto start = std::chrono::high_resolution_clock::now();
    MiniBlock newBlock(chain.size(), chain.back().hash, txs);
    newBlock.mineBlock(difficulty);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    chain.push_back(newBlock);
    std::cout << "Added Mini Block #" << newBlock.index << " in " << duration.count() << " ms" << std::endl;
    newBlock.displayBlock();
}

bool MiniBlockchain::isValid() const {
    for (size_t i = 1; i < chain.size(); i++) {
        if (chain[i].hash != chain[i].calculateHash()) return false;
        if (chain[i].previousHash != chain[i - 1].hash) return false;
    }
    return true;
}

void MiniBlockchain::displayChain() const {
    for (const auto& block : chain) {
        block.displayBlock();
    }
}

void MiniBlockchain::setDifficulty(int diff) {
    difficulty = diff;
}