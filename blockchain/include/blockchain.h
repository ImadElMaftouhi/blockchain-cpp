#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <vector>
#include <ctime>
#include <openssl/sha.h>
#include "merkle_tree.h"
#include "pow.h"
#include "pos.h"

struct Transaction {
    std::string id;
    std::string sender;
    std::string receiver;
    double amount;

    Transaction(std::string i, std::string s, std::string r, double a)
        : id(i), sender(s), receiver(r), amount(a) {}

    std::string toString() const {
        return id + sender + receiver + std::to_string(amount);
    }
};

class MiniBlock : public Block {
public:
    std::string merkleRoot;
    std::vector<Transaction> transactions;

    MiniBlock(int idx, const std::string& prevHash, const std::vector<Transaction>& txs);
    std::string calculateHash() const;
    void mineBlock(int difficulty);
    void displayBlock() const;
};

class MiniBlockchain {
private:
    std::vector<MiniBlock> chain;
    int difficulty;

public:
    MiniBlockchain(int diff = 2);
    void addBlock(const std::vector<Transaction>& txs);
    bool isValid() const;
    void displayChain() const;
    void setDifficulty(int diff);
};

#endif