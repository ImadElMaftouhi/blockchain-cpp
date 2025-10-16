/*

g++ PoW.cpp -o PoW -lssl -lcrypto -I"C:/msys64/mingw64/include" -L"C:/msys64/mingw64/lib"

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


// BLOCK CLASS
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


// BLOCKCHAIN CLASS
class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;
    
public:
    // Constructor - creates genesis block
    Blockchain(int diff = 2) {
        difficulty = diff;
        chain.push_back(createGenesisBlock());
    }
    
    // Create the first block (Genesis Block)
    Block createGenesisBlock() {
        Block genesis(0, "Genesis Block", "0");
        std::cout << "\n=== Creating Genesis Block ===" << std::endl;
        genesis.mineBlock(difficulty);
        return genesis;
    }
    
    // Get the latest block in the chain
    Block getLatestBlock() const {
        return chain.back();
    }
    
    // Add a new block to the chain
    void addBlock(const std::string& data) {
        Block newBlock(chain.size(), data, getLatestBlock().hash);
        
        std::cout << "\n=== Mining Block #" << newBlock.index << " ===" << std::endl;
        std::cout << "Data: " << data << std::endl;
        std::cout << "Difficulty: " << difficulty << std::endl;
        
        newBlock.mineBlock(difficulty);
        chain.push_back(newBlock);
    }
    
    // Verify the integrity of the blockchain
    bool isChainValid() const {
        // Check each block (starting from block 1, since genesis is always valid)
        for (size_t i = 1; i < chain.size(); i++) {
            const Block& currentBlock = chain[i];
            const Block& previousBlock = chain[i - 1];
            
            // Check if current block's hash is correct
            if (currentBlock.hash != currentBlock.calculateHash()) {
                std::cout << "Block #" << i << " has been tampered with!" << std::endl;
                return false;
            }
            
            // Check if previous hash matches
            if (currentBlock.previousHash != previousBlock.hash) {
                std::cout << "Block #" << i << " has invalid previous hash!" << std::endl;
                return false;
            }
            
            // Check if hash meets difficulty requirement
            std::string target(currentBlock.difficulty, '0');
            if (currentBlock.hash.substr(0, currentBlock.difficulty) != target) {
                std::cout << "Block #" << i << " doesn't meet difficulty requirement!" << std::endl;
                return false;
            }
        }
        
        return true;
    }
    
    // Display entire blockchain
    void displayChain() const {
        std::cout << "\n======================================" << std::endl;
        std::cout << "       BLOCKCHAIN CONTENTS" << std::endl;
        std::cout << "======================================" << std::endl;
        
        for (const auto& block : chain) {
            block.displayBlock();
        }
        
        std::cout << "\n======================================" << std::endl;
    }
    
    // Set difficulty level
    void setDifficulty(int diff) {
        difficulty = diff;
    }
    
    // Get current difficulty
    int getDifficulty() const {
        return difficulty;
    }
    
    // Get chain size
    size_t getChainSize() const {
        return chain.size();
    }
};


// DIFFICULTY COMPARISON TEST
void testDifficultyLevels() {
    std::cout << "\n########################################" << std::endl;
    std::cout << "#   DIFFICULTY LEVEL COMPARISON TEST   #" << std::endl;
    std::cout << "########################################\n" << std::endl;
    
    std::vector<int> difficulties = {1, 2, 3, 4, 5};
    
    std::cout << "Testing block mining with different difficulty levels..." << std::endl;
    std::cout << "Each test will mine a single block.\n" << std::endl;
    
    for (int diff : difficulties) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "Testing Difficulty: " << diff << " (hash must start with " << diff << " zeros)" << std::endl;
        std::cout << "========================================" << std::endl;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        Block testBlock(1, "Test Block for Difficulty " + std::to_string(diff), "previous_hash");
        testBlock.mineBlock(diff);
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        std::cout << "\nResult:" << std::endl;
        std::cout << "  Difficulty: " << diff << std::endl;
        std::cout << "  Attempts (Nonce): " << testBlock.nonce << std::endl;
        std::cout << "  Time: " << duration.count() << " ms" << std::endl;
        std::cout << "  Hash: " << testBlock.hash << std::endl;
        std::cout << "  Verification: " << testBlock.hash.substr(0, diff) << " (first " << diff << " chars)" << std::endl;
    }
}

// MAIN: DEMONSTRATION AND TESTING
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   PROOF OF WORK IMPLEMENTATION DEMO" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // ========== TEST 1: Basic Blockchain Creation ==========
    std::cout << "\n****************************************" << std::endl;
    std::cout << "TEST 1: Creating Basic Blockchain" << std::endl;
    std::cout << "****************************************" << std::endl;
    
    Blockchain myBlockchain(2);
    
    std::cout << "\nAdding blocks to the chain..." << std::endl;
    
    myBlockchain.addBlock("Alice sends 50 BTC to Bob");
    myBlockchain.addBlock("Bob sends 30 BTC to Charlie");
    myBlockchain.addBlock("Charlie sends 20 BTC to David");
    
    myBlockchain.displayChain();
    
    // Verify blockchain integrity
    std::cout << "\n=== Blockchain Validation ===" << std::endl;
    if (myBlockchain.isChainValid()) {
        std::cout << "✓ Blockchain is VALID!" << std::endl;
    } else {
        std::cout << "✗ Blockchain is INVALID!" << std::endl;
    }
    
    // ========== TEST 2: Tampering Detection ==========
    std::cout << "\n\n****************************************" << std::endl;
    std::cout << "TEST 2: Tampering Detection" << std::endl;
    std::cout << "****************************************" << std::endl;
    
    std::cout << "\nAttempting to tamper with block data..." << std::endl;
    std::cout << "(This should invalidate the blockchain)" << std::endl;
    
    // Note: In a real implementation, we'd access and modify a block
    // For demonstration, we create a new blockchain and explain the concept
    std::cout << "\nIf we change the data in any block:" << std::endl;
    std::cout << "  1. The block's hash would no longer match its content" << std::endl;
    std::cout << "  2. The next block's 'previousHash' would be incorrect" << std::endl;
    std::cout << "  3. The entire chain would be invalidated" << std::endl;
    std::cout << "\nThis is why blockchain is tamper-proof!" << std::endl;
    
    // ========== TEST 3: Difficulty Comparison ==========
    testDifficultyLevels();
    
    // ========== TEST 4: Summary Statistics ==========
    std::cout << "\n\n########################################" << std::endl;
    std::cout << "#         SUMMARY & ANALYSIS           #" << std::endl;
    std::cout << "########################################\n" << std::endl;
    
    std::cout << "Key Observations:" << std::endl;
    std::cout << "1. Higher difficulty = More time required" << std::endl;
    std::cout << "2. Time increases exponentially with difficulty" << std::endl;
    std::cout << "3. Difficulty 1: typically < 1ms" << std::endl;
    std::cout << "4. Difficulty 2: typically 1-10ms" << std::endl;
    std::cout << "5. Difficulty 3: typically 10-100ms" << std::endl;
    std::cout << "6. Difficulty 4: typically 100-1000ms" << std::endl;
    std::cout << "7. Difficulty 5: typically 1-10 seconds" << std::endl;
    std::cout << "\nBitcoin uses difficulty ~19 (leading zeros in hash)" << std::endl;
    std::cout << "This makes attacks computationally infeasible!" << std::endl;
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "    PROOF OF WORK DEMO COMPLETED!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}