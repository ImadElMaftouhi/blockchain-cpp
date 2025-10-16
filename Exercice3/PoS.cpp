/* proof_of_stake.c++

g++ PoS.cpp -o PoS -lssl -lcrypto -I"C:/msys64/mingw64/include" -L"C:/msys64/mingw64/lib"
*/

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <openssl/sha.h>

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// BLOCK CLASS (for PoS)
class BlockPoS {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    std::string validator;  // Validator who created this block
    
    BlockPoS(int idx, const std::string& blockData, const std::string& prevHash, const std::string& validatorName) {
        index = idx;
        data = blockData;
        previousHash = prevHash;
        validator = validatorName;
        timestamp = getCurrentTime();
        hash = calculateHash();
    }
    
    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << timestamp << data << previousHash << validator;
        return sha256(ss.str());
    }
    
    void displayBlock() const {
        std::cout << "\n--- Block #" << index << " (PoS) ---" << std::endl;
        std::cout << "Timestamp: " << timestamp << std::endl;
        std::cout << "Data: " << data << std::endl;
        std::cout << "Validator: " << validator << std::endl;
        std::cout << "Previous Hash: " << previousHash.substr(0, 16) << "..." << std::endl;
        std::cout << "Hash: " << hash.substr(0, 16) << "..." << std::endl;
    }
    
private:
    std::string getCurrentTime() const {
        time_t now = time(0);
        char* dt = ctime(&now);
        std::string timeStr(dt);
        timeStr.pop_back();
        return timeStr;
    }
};

// BLOCK CLASS (for PoW - for comparison)
class BlockPoW {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    int nonce;
    int difficulty;
    
    BlockPoW(int idx, const std::string& blockData, const std::string& prevHash) {
        index = idx;
        data = blockData;
        previousHash = prevHash;
        timestamp = getCurrentTime();
        nonce = 0;
        difficulty = 0;
        hash = calculateHash();
    }
    
    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << timestamp << data << previousHash << nonce;
        return sha256(ss.str());
    }
    
    void mineBlock(int difficulty) {
        this->difficulty = difficulty;
        std::string target(difficulty, '0');
        
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateHash();
        }
    }
    
    void displayBlock() const {
        std::cout << "\n--- Block #" << index << " (PoW) ---" << std::endl;
        std::cout << "Timestamp: " << timestamp << std::endl;
        std::cout << "Data: " << data << std::endl;
        std::cout << "Nonce: " << nonce << std::endl;
        std::cout << "Difficulty: " << difficulty << std::endl;
        std::cout << "Previous Hash: " << previousHash.substr(0, 16) << "..." << std::endl;
        std::cout << "Hash: " << hash.substr(0, 16) << "..." << std::endl;
    }
    
private:
    std::string getCurrentTime() const {
        time_t now = time(0);
        char* dt = ctime(&now);
        std::string timeStr(dt);
        timeStr.pop_back();
        return timeStr;
    }
};