#include "pow.h"
#include "utils.h"
#include <sstream>

std::string ProofOfWork::mineBlock(const std::string& data, const std::string& previousHash, int difficulty, int& nonce) {
    std::string target(difficulty, '0');
    std::string blockData = data + previousHash + std::to_string(nonce);
    std::string hash = sha256(blockData);

    while (hash.substr(0, difficulty) != target) {
        nonce++;
        blockData = data + previousHash + std::to_string(nonce);
        hash = sha256(blockData);
    }
    return hash;
}

bool ProofOfWork::verifyBlock(const std::string& data, const std::string& previousHash, const std::string& hash, int difficulty, int nonce) {
    std::string target(difficulty, '0');
    std::string blockData = data + previousHash + std::to_string(nonce);
    std::string calculatedHash = sha256(blockData);
    return calculatedHash == hash && calculatedHash.substr(0, difficulty) == target;
}