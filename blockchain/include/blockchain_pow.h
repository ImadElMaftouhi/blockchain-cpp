#ifndef BLOCKCHAIN_POW_H
#define BLOCKCHAIN_POW_H

#include "blockchain.h"
#include "block_pow.h"
#include "merkle_tree.h"
#include <vector>
#include <string>

class BlockchainPow : public Blockchain {
private:
    std::vector<BlockPow*> chain;

public:
    BlockchainPow(int diff = 2);
    ~BlockchainPow() override;
    void addBlock(const std::vector<std::string>& transactions) override;
    bool isChainValid() const;
    void displayChain() const;
    void setDifficulty(int diff);
    std::string getLatestHash() const override;
};

#endif