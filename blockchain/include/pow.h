#ifndef POW_H
#define POW_H

#include <string>

class ProofOfWork {
public:
    static std::string mineBlock(const std::string& data, const std::string& previousHash, int difficulty, int& nonce);
    static bool verifyBlock(const std::string& data, const std::string& previousHash, const std::string& hash, int difficulty, int nonce);
};

#endif