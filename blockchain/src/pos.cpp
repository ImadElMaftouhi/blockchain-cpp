#include "pos.h"
#include "utils.h"
#include <random>
#include <sstream>

std::string ProofOfStake::validateBlock(const std::string& data, const std::string& previousHash, std::vector<Validator>& validators, std::string& selectedValidator) {
    int totalStake = 0;
    for (const auto& v : validators) totalStake += v.stake;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, totalStake - 1);
    int random = dis(gen);
    int cumulative = 0;
    for (const auto& v : validators) {
        cumulative += v.stake;
        if (random < cumulative) {
            selectedValidator = v.id;
            break;
        }
    }
    std::stringstream ss;
    ss << data << previousHash << selectedValidator;
    return sha256(ss.str());
}

bool ProofOfStake::verifyBlock(const std::string& data, const std::string& previousHash, const std::string& hash, const std::string& validator) {
    std::stringstream ss;
    ss << data << previousHash << validator;
    std::string calculatedHash = sha256(ss.str());
    return calculatedHash == hash;
}