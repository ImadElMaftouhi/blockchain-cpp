#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include <string>
#include "block.h"
#include "merkle_tree.h"
#include "validator.h"

class Blockchain {
protected:
    std::vector<Block*> chain;
    int difficulty;

public:
    Blockchain(int diff = 2);
    virtual ~Blockchain();
    virtual void addBlock(const std::vector<std::string>& transactions) = 0;
    bool isChainValid() const;
    void displayChain() const;
    void setDifficulty(int diff);
    virtual std::string getLatestHash() const;
};

#endif