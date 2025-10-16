// proof_of_stake.c++
// Imad El Maftouhi

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

class Block {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    int nonce;
    int difficulty;
    
    // Constructor
    Block(int idx, const std::string& blockData, const std::string& prevHash) {
        index = idx;
        data = blockData;
        previousHash = prevHash;
        timestamp = getCurrentTime();
        nonce = 0;
        difficulty = 0;
        hash = calculateHash();
    }
    
    // Calculate hash of the block
    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << timestamp << data << previousHash << nonce;
        return sha256(ss.str());
    }
    
    // Mine block with Proof of Work
    void mineBlock(int difficulty) {
        this->difficulty = difficulty;
        
        // Create target string with 'difficulty' number of leading zeros
        std::string target(difficulty, '0');
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Keep hashing until we find a hash that starts with target zeros
        while (hash.substr(0, difficulty) != target) {
            nonce++;
            hash = calculateHash();
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        std::cout << "Block mined! Hash: " << hash << std::endl;
        std::cout << "Nonce: " << nonce << std::endl;
        std::cout << "Mining time: " << duration.count() << " ms" << std::endl;
    }
    
    // Display block information
    void displayBlock() const {
        std::cout << "\n--- Block #" << index << " ---" << std::endl;
        std::cout << "Timestamp: " << timestamp << std::endl;
        std::cout << "Data: " << data << std::endl;
        std::cout << "Previous Hash: " << previousHash.substr(0, 16) << "..." << std::endl;
        std::cout << "Hash: " << hash.substr(0, 16) << "..." << std::endl;
        std::cout << "Nonce: " << nonce << std::endl;
        std::cout << "Difficulty: " << difficulty << std::endl;
    }
    
private:
    // Get current time as string
    std::string getCurrentTime() const {
        time_t now = time(0);
        char* dt = ctime(&now);
        std::string timeStr(dt);
        timeStr.pop_back(); // Remove newline
        return timeStr;
    }
};

