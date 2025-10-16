#ifndef BLOCKCHAIN_POS_H
#define BLOCKCHAIN_POS_H

#include "blockchain.h"
#include "block_pos.h"
#include "merkle_tree.h"
#include "validator.h"
#include <vector>
#include <string>

class BlockchainPos : public Blockchain {
private:
    std::vector<BlockPos*> chain;
    std::vector<Validator> validators;

public:
    BlockchainPos(int diff = 2, const std::vector<Validator>& vals = {});
    ~BlockchainPos() override;
    void addBlock(const std::vector<std::string>& transactions) override;
    bool isChainValid() const ;
    void displayChain() const ;
    void setDifficulty(int diff) ;
    std::string getLatestHash() const override;
    void setValidators(const std::vector<Validator>& vals);
};

#endif