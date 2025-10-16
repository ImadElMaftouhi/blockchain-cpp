#include "block_pow.h"
#include "utils.h"
#include "pow.h"
#include <iostream>
#include <sstream>

BlockPow::BlockPow(int idx, const std::string& prevHash, const std::string& h, const std::string& d, int n, int diff)
    : index(idx), previousHash(prevHash), hash(h), data(d), nonce(n), difficulty(diff) {}

BlockPow::~BlockPow() {}

std::string BlockPow::getHash() const {
    return hash;
}

std::string BlockPow::getPreviousHash() const {
    return previousHash;
}

int BlockPow::getIndex() const {
    return index;
}

std::string BlockPow::calculateHash() const {
    std::stringstream ss;
    ss << index << getCurrentTime() << data << previousHash << nonce;
    return sha256(ss.str());
}

void BlockPow::display() const {
    std::cout << "\n--- Block #" << index << " (PoW) ---" << std::endl;
    std::cout << "Timestamp: " << getCurrentTime() << std::endl;
    std::cout << "Data: " << data << std::endl;
    std::cout << "Previous Hash: " << previousHash.substr(0, 16) << "..." << std::endl;
    std::cout << "Hash: " << hash.substr(0, 16) << "..." << std::endl;
    std::cout << "Nonce: " << nonce << std::endl;
    std::cout << "Difficulty: " << difficulty << std::endl;
}

std::string BlockPow::getData() const {
    return data;
}

int BlockPow::getNonce() const {
    return nonce;
}

int BlockPow::getDifficulty() const {
    return difficulty;
}