#ifndef POS_H
#define POS_H

#include <string>
#include <vector>
#include "validator.h"

class ProofOfStake {
public:
    static std::string validateBlock(const std::string& data, const std::string& previousHash, std::vector<Validator>& validators, std::string& selectedValidator);
    static bool verifyBlock(const std::string& data, const std::string& previousHash, const std::string& hash, const std::string& validator);
};

#endif