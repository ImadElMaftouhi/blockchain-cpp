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
#include <random>
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

// Validator class for Proof of Stake
class Validator {
public:
    std::string id;
    int stake;

    Validator(std::string i, int s) : id(i), stake(s) {}
};

// Block class for Proof of Stake
class BlockPoS {
public:
    int index;
    std::string timestamp;
    std::string data;
    std::string previousHash;
    std::string hash;
    std::string validator;

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

// Block class for Proof of Work (for comparison)
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

// Blockchain class for PoS
class PoSBlockchain {
private:
    std::vector<BlockPoS> chain;
    std::vector<Validator> validators;

    std::string selectValidator() {
        int totalStake = 0;
        for (const auto& v : validators) totalStake += v.stake;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, totalStake - 1);
        int random = dis(gen);
        int cumulative = 0;
        for (const auto& v : validators) {
            cumulative += v.stake;
            if (random < cumulative) return v.id;
        }
        return validators.back().id;
    }

public:
    PoSBlockchain(const std::vector<Validator>& vals) : validators(vals) {
        chain.emplace_back(0, "Genesis Block", "0", "GenesisValidator");
    }

    void addBlock(const std::string& data) {
        auto start = std::chrono::high_resolution_clock::now();
        std::string validator = selectValidator();
        BlockPoS newBlock(chain.size(), data, chain.back().hash, validator);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        chain.push_back(newBlock);
        std::cout << "Added PoS Block #" << newBlock.index << " by " << validator << " in " << duration.count() << " ms" << std::endl;
        newBlock.displayBlock();
    }

    bool isChainValid() const {
        for (size_t i = 1; i < chain.size(); i++) {
            if (chain[i].hash != chain[i].calculateHash()) return false;
            if (chain[i].previousHash != chain[i - 1].hash) return false;
        }
        return true;
    }
};

// Blockchain class for PoW (for comparison)
class PoWBlockchain {
private:
    std::vector<BlockPoW> chain;

public:
    PoWBlockchain() {
        chain.emplace_back(0, "Genesis Block", "0");
    }

    void addBlock(const std::string& data, int difficulty) {
        auto start = std::chrono::high_resolution_clock::now();
        BlockPoW newBlock(chain.size(), data, chain.back().hash);
        newBlock.mineBlock(difficulty);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        chain.push_back(newBlock);
        std::cout << "Added PoW Block #" << newBlock.index << " with difficulty " << difficulty << " in " << duration.count() << " ms" << std::endl;
        newBlock.displayBlock();
    }

    bool isChainValid() const {
        for (size_t i = 1; i < chain.size(); i++) {
            if (chain[i].hash != chain[i].calculateHash()) return false;
            if (chain[i].previousHash != chain[i - 1].hash) return false;
        }
        return true;
    }
};

int main() {
    std::vector<Validator> validators = {{"V1", 50}, {"V2", 30}, {"V3", 20}};
    PoSBlockchain posChain(validators);
    PoWBlockchain powChain;

    std::cout << "=== Proof of Stake (PoS) Demonstration ===" << std::endl;
    posChain.addBlock("Transaction 1: 50 BTC to Alice");
    posChain.addBlock("Transaction 2: 30 BTC to Bob");
    std::cout << "PoS Chain Valid: " << (posChain.isChainValid() ? "Yes" : "No") << std::endl;

    std::cout << "\n=== Proof of Work (PoW) Demonstration ===" << std::endl;
    powChain.addBlock("Transaction 1: 50 BTC to Alice", 4);
    powChain.addBlock("Transaction 2: 30 BTC to Bob", 4);
    std::cout << "PoW Chain Valid: " << (powChain.isChainValid() ? "Yes" : "No") << std::endl;

    return 0;
}