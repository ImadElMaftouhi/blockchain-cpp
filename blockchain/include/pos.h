#ifndef POS_H
#define POS_H

#include <string>
#include <vector>

class Validator {
public:
    std::string id;
    int stake;

    Validator(std::string i, int s);
};

class BlockPoS {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    std::string validator;

    BlockPoS(int idx, const std::string& blockData, const std::string& prevHash, const std::string& validatorName);
    std::string calculateHash() const;
    void displayBlock() const;
private:
    std::string getCurrentTime() const;
};

class BlockPoW {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    int nonce;
    int difficulty;

    BlockPoW(int idx, const std::string& blockData, const std::string& prevHash);
    std::string calculateHash() const;
    void mineBlock(int difficulty);
    void displayBlock() const;
private:
    std::string getCurrentTime() const;
};

class PoSBlockchain {
private:
    std::vector<BlockPoS> chain;
    std::vector<Validator> validators;

    std::string selectValidator();

public:
    PoSBlockchain(const std::vector<Validator>& vals);
    void addBlock(const std::string& data);
    bool isChainValid() const;
    void displayChain() const;
};

class PoWBlockchain {
private:
    std::vector<BlockPoW> chain;

public:
    PoWBlockchain();
    void addBlock(const std::string& data, int difficulty);
    bool isChainValid() const;
    void displayChain() const;
};

#endif