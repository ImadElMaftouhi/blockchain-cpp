#ifndef BLOCK_POW_H
#define BLOCK_POW_H

#include "block.h"
#include <string>

class BlockPow : public Block {
private:
    int index;
    std::string previousHash;
    std::string hash;
    std::string data;
    int nonce;
    int difficulty;

public:
    BlockPow(int idx, const std::string& prevHash, const std::string& h, const std::string& d, int n, int diff);
    ~BlockPow() override;
    std::string getHash() const override;
    std::string getPreviousHash() const override;
    int getIndex() const override;
    std::string calculateHash() const override;
    void display() const override;
    std::string getData() const;
    int getNonce() const;
    int getDifficulty() const;
};

#endif