#ifndef POW_H
#define POW_H

#include <string>
#include <vector>

class Block {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    int nonce;
    int difficulty;

    Block(int idx, const std::string& blockData, const std::string& prevHash);
    std::string calculateHash() const;
    void mineBlock(int difficulty);
    void displayBlock() const;
private:
    std::string getCurrentTime() const;
};

class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;

public:
    Blockchain(int diff = 2);
    Block createGenesisBlock();
    Block getLatestBlock() const;
    void addBlock(const std::string& data);
    bool isChainValid() const;
    void displayChain() const;
    void setDifficulty(int diff);
    int getDifficulty() const;
    size_t getChainSize() const;
};

void testDifficultyLevels();

#endif