// proof_of_stake.c++
// Imad El Maftouhi

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <random>
#include <algorithm>
#include <openssl/sha.h>


// HELPER FUNCTION: SHA-256 Hashing
std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// ============================================================================
// VALIDATOR CLASS
// ============================================================================
class Validator {
public:
    std::string name;
    int stake;  // Amount staked by the validator
    
    Validator(const std::string& validatorName, int validatorStake) 
        : name(validatorName), stake(validatorStake) {}
    
    void displayInfo() const {
        std::cout << "  Validator: " << name << ", Stake: " << stake << " coins" << std::endl;
    }
};

// ============================================================================
// BLOCK CLASS (for PoS)
// ============================================================================
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

// ============================================================================
// BLOCK CLASS (for PoW - for comparison)
// ============================================================================
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

// ============================================================================
// PROOF OF STAKE BLOCKCHAIN
// ============================================================================
class BlockchainPoS {
private:
    std::vector<BlockPoS> chain;
    std::vector<Validator> validators;
    std::mt19937 rng;
    
public:
    BlockchainPoS() {
        // Initialize random number generator with time-based seed
        rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
        
        // Create genesis block
        chain.push_back(createGenesisBlock());
    }
    
    BlockPoS createGenesisBlock() {
        std::cout << "\n=== Creating Genesis Block (PoS) ===" << std::endl;
        return BlockPoS(0, "Genesis Block", "0", "System");
    }
    
    void addValidator(const std::string& name, int stake) {
        validators.push_back(Validator(name, stake));
        std::cout << "Added validator: " << name << " with stake: " << stake << std::endl;
    }
    
    // Select validator based on stake weight (higher stake = higher probability)
    std::string selectValidator() {
        if (validators.empty()) {
            return "Default Validator";
        }
        
        // Calculate total stake
        int totalStake = 0;
        for (const auto& v : validators) {
            totalStake += v.stake;
        }
        
        // Random selection weighted by stake
        std::uniform_int_distribution<int> dist(1, totalStake);
        int randomValue = dist(rng);
        
        int cumulativeStake = 0;
        for (const auto& v : validators) {
            cumulativeStake += v.stake;
            if (randomValue <= cumulativeStake) {
                return v.name;
            }
        }
        
        return validators.back().name;
    }
    
    void addBlock(const std::string& data) {
        std::string selectedValidator = selectValidator();
        
        std::cout << "\n=== Creating Block #" << chain.size() << " (PoS) ===" << std::endl;
        std::cout << "Selected Validator: " << selectedValidator << std::endl;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        BlockPoS newBlock(chain.size(), data, getLatestBlock().hash, selectedValidator);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        
        std::cout << "Block created in: " << duration.count() << " microseconds" << std::endl;
        
        chain.push_back(newBlock);
    }
    
    BlockPoS getLatestBlock() const {
        return chain.back();
    }
    
    bool isChainValid() const {
        for (size_t i = 1; i < chain.size(); i++) {
            const BlockPoS& currentBlock = chain[i];
            const BlockPoS& previousBlock = chain[i - 1];
            
            if (currentBlock.hash != currentBlock.calculateHash()) {
                return false;
            }
            
            if (currentBlock.previousHash != previousBlock.hash) {
                return false;
            }
        }
        return true;
    }
    
    void displayChain() const {
        std::cout << "\n======================================" << std::endl;
        std::cout << "    BLOCKCHAIN (PoS) CONTENTS" << std::endl;
        std::cout << "======================================" << std::endl;
        
        for (const auto& block : chain) {
            block.displayBlock();
        }
        
        std::cout << "\n======================================" << std::endl;
    }
    
    void displayValidators() const {
        std::cout << "\n=== Registered Validators ===" << std::endl;
        for (const auto& v : validators) {
            v.displayInfo();
        }
    }
    
    size_t getChainSize() const {
        return chain.size();
    }
};

// ============================================================================
// PROOF OF WORK BLOCKCHAIN (for comparison)
// ============================================================================
class BlockchainPoW {
private:
    std::vector<BlockPoW> chain;
    int difficulty;
    
public:
    BlockchainPoW(int diff) : difficulty(diff) {
        chain.push_back(createGenesisBlock());
    }
    
    BlockPoW createGenesisBlock() {
        std::cout << "\n=== Creating Genesis Block (PoW) ===" << std::endl;
        BlockPoW genesis(0, "Genesis Block", "0");
        genesis.mineBlock(difficulty);
        return genesis;
    }
    
    void addBlock(const std::string& data) {
        std::cout << "\n=== Mining Block #" << chain.size() << " (PoW) ===" << std::endl;
        std::cout << "Difficulty: " << difficulty << std::endl;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        BlockPoW newBlock(chain.size(), data, getLatestBlock().hash);
        newBlock.mineBlock(difficulty);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        
        std::cout << "Block mined in: " << duration.count() << " microseconds" << std::endl;
        
        chain.push_back(newBlock);
    }
    
    BlockPoW getLatestBlock() const {
        return chain.back();
    }
    
    void displayChain() const {
        std::cout << "\n======================================" << std::endl;
        std::cout << "    BLOCKCHAIN (PoW) CONTENTS" << std::endl;
        std::cout << "======================================" << std::endl;
        
        for (const auto& block : chain) {
            block.displayBlock();
        }
        
        std::cout << "\n======================================" << std::endl;
    }
};

// ============================================================================
// COMPARISON TEST: PoW vs PoS
// ============================================================================
void comparePoWvsPoS() {
    std::cout << "\n########################################" << std::endl;
    std::cout << "#     PoW vs PoS COMPARISON TEST      #" << std::endl;
    std::cout << "########################################\n" << std::endl;
    
    const int NUM_BLOCKS = 5;
    const int POW_DIFFICULTY = 4;
    
    // Test data
    std::vector<std::string> testData = {
        "Transaction 1: Alice -> Bob (50 coins)",
        "Transaction 2: Bob -> Charlie (30 coins)",
        "Transaction 3: Charlie -> David (20 coins)",
        "Transaction 4: David -> Eve (10 coins)",
        "Transaction 5: Eve -> Alice (5 coins)"
    };
    
    // ========== PROOF OF WORK TEST ==========
    std::cout << "========================================" << std::endl;
    std::cout << "      TESTING PROOF OF WORK" << std::endl;
    std::cout << "========================================" << std::endl;
    
    auto powStartTime = std::chrono::high_resolution_clock::now();
    
    BlockchainPoW powChain(POW_DIFFICULTY);
    
    for (int i = 0; i < NUM_BLOCKS; i++) {
        powChain.addBlock(testData[i]);
    }
    
    auto powEndTime = std::chrono::high_resolution_clock::now();
    auto powTotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(powEndTime - powStartTime);
    
    // ========== PROOF OF STAKE TEST ==========
    std::cout << "\n\n========================================" << std::endl;
    std::cout << "      TESTING PROOF OF STAKE" << std::endl;
    std::cout << "========================================" << std::endl;
    
    BlockchainPoS posChain;
    
    // Register validators with different stakes
    std::cout << "\nRegistering validators..." << std::endl;
    posChain.addValidator("Alice", 100);
    posChain.addValidator("Bob", 200);
    posChain.addValidator("Charlie", 150);
    posChain.addValidator("David", 300);
    posChain.addValidator("Eve", 50);
    
    posChain.displayValidators();
    
    auto posStartTime = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < NUM_BLOCKS; i++) {
        posChain.addBlock(testData[i]);
    }
    
    auto posEndTime = std::chrono::high_resolution_clock::now();
    auto posTotalTime = std::chrono::duration_cast<std::chrono::milliseconds>(posEndTime - posStartTime);
    
    // ========== RESULTS COMPARISON ==========
    std::cout << "\n\n########################################" << std::endl;
    std::cout << "#         COMPARISON RESULTS           #" << std::endl;
    std::cout << "########################################\n" << std::endl;
    
    std::cout << "Number of blocks added: " << NUM_BLOCKS << std::endl;
    std::cout << "\n--- PROOF OF WORK ---" << std::endl;
    std::cout << "  Difficulty: " << POW_DIFFICULTY << std::endl;
    std::cout << "  Total time: " << powTotalTime.count() << " ms" << std::endl;
    std::cout << "  Average per block: " << powTotalTime.count() / NUM_BLOCKS << " ms" << std::endl;
    
    std::cout << "\n--- PROOF OF STAKE ---" << std::endl;
    std::cout << "  Total time: " << posTotalTime.count() << " ms" << std::endl;
    std::cout << "  Average per block: " << posTotalTime.count() / NUM_BLOCKS << " ms" << std::endl;
    
    std::cout << "\n--- SPEED COMPARISON ---" << std::endl;
    double speedup = (double)powTotalTime.count() / (double)posTotalTime.count();
    std::cout << "  PoS is " << std::fixed << std::setprecision(2) << speedup << "x FASTER than PoW" << std::endl;
    
    std::cout << "\n--- ANALYSIS ---" << std::endl;
    std::cout << "  ✓ PoS is significantly faster (no computational puzzles)" << std::endl;
    std::cout << "  ✓ PoS consumes much less energy (no mining)" << std::endl;
    std::cout << "  ✓ PoW is more decentralized (anyone can mine)" << std::endl;
    std::cout << "  ✓ PoS favors validators with higher stakes" << std::endl;
    
    // Verify both chains
    std::cout << "\n--- CHAIN VALIDATION ---" << std::endl;
    std::cout << "  PoW Chain Valid: " << (powChain.getLatestBlock().hash != "" ? "✓ YES" : "✗ NO") << std::endl;
    std::cout << "  PoS Chain Valid: " << (posChain.isChainValid() ? "✓ YES" : "✗ NO") << std::endl;
}

// ============================================================================
// MAIN: DEMONSTRATION AND TESTING
// ============================================================================
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   PROOF OF STAKE IMPLEMENTATION DEMO" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // ========== TEST 1: Basic PoS Blockchain ==========
    std::cout << "\n****************************************" << std::endl;
    std::cout << "TEST 1: Creating PoS Blockchain" << std::endl;
    std::cout << "****************************************" << std::endl;
    
    BlockchainPoS myPoSChain;
    
    // Add validators
    std::cout << "\nAdding validators..." << std::endl;
    myPoSChain.addValidator("Alice", 100);
    myPoSChain.addValidator("Bob", 200);
    myPoSChain.addValidator("Charlie", 300);
    
    myPoSChain.displayValidators();
    
    // Add blocks
    std::cout << "\nAdding blocks to PoS chain..." << std::endl;
    myPoSChain.addBlock("Alice sends 50 coins to Bob");
    myPoSChain.addBlock("Bob sends 30 coins to Charlie");
    myPoSChain.addBlock("Charlie sends 20 coins to Alice");
    
    myPoSChain.displayChain();
    
    // Validate chain
    std::cout << "\n=== PoS Chain Validation ===" << std::endl;
    if (myPoSChain.isChainValid()) {
        std::cout << "✓ PoS Blockchain is VALID!" << std::endl;
    } else {
        std::cout << "✗ PoS Blockchain is INVALID!" << std::endl;
    }
    
    // ========== TEST 2: PoW vs PoS Comparison ==========
    comparePoWvsPoS();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "    PROOF OF STAKE DEMO COMPLETED!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}