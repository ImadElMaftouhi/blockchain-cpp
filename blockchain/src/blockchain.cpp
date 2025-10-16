#include "blockchain.h"
#include "utils.h"
#include <iostream>

Blockchain::Blockchain(int diff) : difficulty(diff) {
    chain.push_back(nullptr); // Placeholder for genesis block
}

Blockchain::~Blockchain() {
    for (auto* block : chain) {
        delete block;
    }
}

bool Blockchain::isChainValid() const {
    for (size_t i = 1; i < chain.size(); i++) {
        if (chain[i] == nullptr) continue;
        if (chain[i]->getHash() != chain[i]->calculateHash()) return false;
        if (chain[i]->getPreviousHash() != chain[i-1]->getHash()) return false;
    }
    return true;
}

void Blockchain::displayChain() const {
    for (const auto* block : chain) {
        if (block) block->display();
    }
}

void Blockchain::setDifficulty(int diff) {
    difficulty = diff;
}

std::string Blockchain::getLatestHash() const {
    return chain.empty() ? "0" : chain.back()->getHash();
}