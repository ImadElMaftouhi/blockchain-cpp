#include "blockchain_pos.h"
#include "utils.h"
#include "pos.h"
#include <iostream>
#include <chrono>

BlockchainPos::BlockchainPos(int diff, const std::vector<Validator>& vals) : Blockchain(diff) {
    validators = vals;
    std::string genesisData = "Genesis Block";
    std::string selectedValidator = "GenesisValidator";
    std::string genesisHash = ProofOfStake::validateBlock(genesisData, "0", validators, selectedValidator);
    chain.push_back(new BlockPos(0, "0", genesisHash, genesisData, selectedValidator));
}

BlockchainPos::~BlockchainPos() {
    for (auto* block : chain) {
        delete block;
    }
}

void BlockchainPos::addBlock(const std::vector<std::string>& transactions) {
    MerkleTree merkleTree(transactions);
    std::string merkleRoot = merkleTree.getRootHash();
    std::string prevHash = getLatestHash();
    std::string selectedValidator;
    auto start = std::chrono::high_resolution_clock::now();
    std::string newHash = ProofOfStake::validateBlock(merkleRoot, prevHash, validators, selectedValidator);
    auto end = std::chrono::high_resolution_clock::now();
    long long duration = measureTime([&]() {});
    chain.push_back(new BlockPos(chain.size(), prevHash, newHash, merkleRoot, selectedValidator));
    std::cout << "Block #" << chain.size() - 1 << " validated by " << selectedValidator << " in " << duration << " ms" << std::endl;
}

bool BlockchainPos::isChainValid() const {
    for (size_t i = 1; i < chain.size(); i++) {
        if (!ProofOfStake::verifyBlock(chain[i]->getData(), chain[i]->getPreviousHash(), chain[i]->getHash(), chain[i]->getValidator())) {
            return false;
        }
        if (chain[i]->getPreviousHash() != chain[i-1]->getHash()) return false;
    }
    return true;
}

void BlockchainPos::displayChain() const {
    for (const auto* block : chain) {
        block->display();
    }
}

void BlockchainPos::setDifficulty(int diff) {
    difficulty = diff;
}

std::string BlockchainPos::getLatestHash() const {
    return chain.empty() ? "0" : chain.back()->getHash();
}

void BlockchainPos::setValidators(const std::vector<Validator>& vals) {
    validators = vals;
}