#ifndef POS_H
#define POS_H

#include <string>
#include <vector>

class Validator {
public:
    std::string id;
    int stake;

    Validator(std::string i, int s) : id(i), stake(s) {}
};

class ProofOfStake {
public:
    static std::string validateBlock(const std::string& data, const std::string& previousHash, std::vector<Validator>& validators, std::string& selectedValidator);
    static bool verifyBlock(const std::string& data, const std::string& previousHash, const std::string& hash, const std::string& validator);
};

#endif