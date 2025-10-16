#ifndef BLOCK_POS_H
#define BLOCK_POS_H

#include "block.h"
#include <string>

class BlockPos : public Block {
private:
    int index;
    std::string previousHash;
    std::string hash;
    std::string data;
    std::string validator;

public:
    BlockPos(int idx, const std::string& prevHash, const std::string& h, const std::string& d, const std::string& v);
    ~BlockPos() override;
    std::string getHash() const override;
    std::string getPreviousHash() const override;
    int getIndex() const override;
    std::string calculateHash() const override;
    void display() const override;
    std::string getData() const;
    std::string getValidator() const;
};

#endif