#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>


std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex                    // Use hexadecimal format
           << std::setw(2)                // Each byte = 2 hex digits
           << std::setfill('0')           // Pad with 0 if needed
           << (int)hash[i];               // Convert byte to int
    }
    return ss.str();
}

class Transaction {
public:
    std::string id;
    std::string sender;
    std::string receiver;
    double amount;

    Transaction(std::string i, std::string s, std::string r, double a)
        : id(i), sender(s), receiver(r), amount(a) {}
};

 // Simple Validator struct used for Proof-of-Stake demo
 struct Validator {
     std::string id;
     int stake;
 };

 // Node class for Merkle Tree (Partie 1 - Step 2)
 // Copy the MerkleNode class from merkle_tree.cpp
 class MerkleNode {
    public:
        std::string hash;
        MerkleNode* left;
        MerkleNode* right;

        MerkleNode(const std::string& data) {
            this->hash =sha256(data);
            this->left = nullptr;
            this->right = nullptr;
        };

        MerkleNode(MerkleNode* l, MerkleNode* r) {
            this->left = l;
            this->right = r;
            std::string combined = l->hash + r->hash;
            this->hash = sha256(combined);
        };

        // Destructor
class MerkleTree {
    private:
        MerkleNode* root;

        MerkleNode* buildTreeRecursive(std::vector<MerkleNode*>& nodes) {
            // base case
            if (nodes.empty()) return nullptr;
            if (nodes.size() == 1){
                return nodes[0];
            }
            std::vector<MerkleNode*> parentNodes;
            for (size_t i=0; i<nodes.size(); i+=2) {
                MerkleNode* left = nodes[i];
                MerkleNode* right;
                if (i+1 == nodes.size()){
                    right = nodes[i];
                } else {
                    right = nodes[i+1];
                }
                MerkleNode* parent = new MerkleNode(left, right);
                parentNodes.push_back(parent);
            }
            return buildTreeRecursive(parentNodes);
        }
    public:
        // Construct from a list of Transaction pointers (original usage in Merkle tree exercises)
        MerkleTree(const std::vector<Transaction*>& transactions){
            if (transactions.empty()){
                root = nullptr;
                return;
            }

            std::vector<MerkleNode*> leafNodes;
            for (const auto& tx : transactions) {
                // tx is a Transaction* so dereference
                std::string txString = tx->id + tx->sender + tx->receiver + std::to_string(tx->amount);
                leafNodes.push_back(new MerkleNode(txString));
            }
            root = buildTreeRecursive(leafNodes);
        }

        // Construct from a list of already-serialized transaction strings (used by Blockchain::addBlockPoW/PoS)
        MerkleTree(const std::vector<std::string>& dataLeaves){
            if (dataLeaves.empty()){
                root = nullptr;
                return;
            }

            std::vector<MerkleNode*> leafNodes;
            for (const auto& s : dataLeaves) {
                leafNodes.push_back(new MerkleNode(s));
            }
            root = buildTreeRecursive(leafNodes);
        }

        std::string getRootHash() const {
            if (root == nullptr){
                return "";
            }
            return root->hash;
        }

        void printTree(MerkleNode* node, int depth=0) const {
            if (node==nullptr) return;
            for (int i=0; i<depth; i++){
                std::cout << "  ";
            }
            std::cout << "Hash: " << node->hash.substr(0, 16) << "..." << std::endl;

            if (node->left !=nullptr || node->right !=nullptr ){
                printTree(node->left, depth+1);
                printTree(node->right, depth+1);
            }
        }

        void printTree() const {
            std::cout << "\n=== Merkle Tree Structure" << std::endl;
            printTree(root);
            std::cout << "========================\n" << std::endl;
        }

        bool verifyTransaction(const Transaction& transaction) const {
            std::string txString = transaction.id + transaction.sender + transaction.receiver + std::to_string(transaction.amount);
            std::string txHash = sha256(txString);
            return searchHash(root, txHash);
        }

    private:
        bool searchHash(MerkleNode* node, const std::string& targetHash) const {
            if (node==nullptr) return false;

            if (node->hash == targetHash) return true;
            return searchHash(node->left, targetHash) || searchHash(node->right, targetHash);
        }

    public:
        ~MerkleTree(){
            delete root;
        }
};

    public:
        ~MerkleTree(){
            delete root;
        }
};



// Block class (Partie 1 - Step 3)
class Block {
public:
    int index;
    std::string timestamp;
    std::string previousHash;
    std::string merkleRoot;
    int nonce;
    std::string hash;
    std::string validator; // For PoS
    int difficulty;       // For PoW

    Block(int idx, const std::string& prevHash, const std::string& merkle)
        : index(idx), previousHash(prevHash), merkleRoot(merkle), nonce(0), difficulty(0) {
        timestamp = getCurrentTime();
        hash = calculateHash();
    }

    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << timestamp << previousHash << merkleRoot << nonce << validator;
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

    void setValidator(const std::string& v) {
        validator = v;
        hash = calculateHash(); // Recalculate hash after setting validator
    }

class Blockchain {
private:
    std::vector<Block> chain;
    std::vector<Validator> validators; // Validators for PoS
    int difficulty;

    std::string selectValidator() {
        int totalStake = 0;
        for (const auto& v : validators) totalStake += v.stake;
        if (totalStake <= 0) return ""; // no validators or zero stake
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
    Blockchain(int diff = 2) : difficulty(diff) {
        chain.emplace_back(0, "0", "Genesis Merkle Root");
    }

    // Allow setting validators from outside
    void setValidators(const std::vector<Validator>& v) {
        validators = v;
    }

    void addBlockPoW(const std::vector<Transaction>& transactions) {
        // Convert transactions to string representations for Merkle Tree
        std::vector<std::string> txStrings;
        for (const auto& tx : transactions) {
            std::stringstream ss;
            ss << tx.id << tx.sender << tx.receiver << tx.amount;
            txStrings.push_back(ss.str());
        }
        MerkleTree merkleTree(txStrings); // Use MerkleTree constructor that takes string leaves
        std::string merkleRoot = merkleTree.getRootHash();

        Block newBlock(chain.size(), chain.back().hash, merkleRoot);
        auto start = std::chrono::high_resolution_clock::now();
        newBlock.mineBlock(difficulty);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Block #" << newBlock.index << " mined in " << duration.count() << " ms" << std::endl;
        chain.push_back(newBlock);
    }

    void addBlockPoS(const std::vector<Transaction>& transactions) {
        std::vector<std::string> txStrings;
        for (const auto& tx : transactions) {
            std::stringstream ss;
            ss << tx.id << tx.sender << tx.receiver << tx.amount;
            txStrings.push_back(ss.str());
        }
        MerkleTree merkleTree(txStrings);
        std::string merkleRoot = merkleTree.getRootHash();

        Block newBlock(chain.size(), chain.back().hash, merkleRoot);
        auto start = std::chrono::high_resolution_clock::now();
        std::string validator = selectValidator();
        newBlock.setValidator(validator);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Block #" << newBlock.index << " validated by " << validator << " in " << duration.count() << " ms" << std::endl;
        chain.push_back(newBlock);
    }

    bool isChainValid() const {
        for (size_t i = 1; i < chain.size(); i++) {
            if (chain[i].hash != chain[i].calculateHash()) return false;
            if (chain[i].previousHash != chain[i - 1].hash) return false;
        }
        return true;
    }

    void displayChain() const {
        for (const auto& block : chain) {
            block.displayBlock();
        }
    }

    void setDifficulty(int diff) {
        difficulty = diff;
    }
};
        std::cout << "Block #" << newBlock.index << " validated by " << validator << " in " << duration.count() << " ms" << std::endl;
        chain.push_back(newBlock);
    }

    bool isChainValid() const {
        for (size_t i = 1; i < chain.size(); i++) {
            if (chain[i].hash != chain[i].calculateHash()) return false;
            if (chain[i].previousHash != chain[i - 1].hash) return false;
        }
    std::vector<Validator> validators = {{"V1", 50}, {"V2", 30}, {"V3", 20}}; // Copy from PoS.cpp
    bc = Blockchain(2); // Reset for PoS demo
    bc.setValidators(validators);
    void displayChain() const {
        for (const auto& block : chain) {
            block.displayBlock();
        }
    }

    void setDifficulty(int diff) {
        difficulty = diff;
    }
};

// Main function for demonstration (Partie 4)
int main() {
    Blockchain bc(2);

    // Partie 4 - Step 1: Add blocks with PoW
    std::vector<Transaction> txsPoW = {
        Transaction("T1", "Alice", "Bob", 50.0),
        Transaction("T2", "Bob", "Charlie", 30.0)
    };
    bc.addBlockPoW(txsPoW);
    bc.setDifficulty(3);
    bc.addBlockPoW(txsPoW);

    // Partie 4 - Step 2: Add blocks with PoS
    std::vector<Validator> validators = {{"V1", 50}, {"V2", 30}, {"V3", 20}}; // Copy from PoS.cpp
    bc = Blockchain(2); // Reset for PoS demo
    bc.validators = validators;
    std::vector<Transaction> txsPoS = {
        Transaction("T3", "Charlie", "David", 20.0),
        Transaction("T4", "David", "Eve", 10.0)
    };
    bc.addBlockPoS(txsPoS);
    bc.addBlockPoS(txsPoS);

    // Partie 4 - Step 3: Display and compare
    std::cout << "\n=== Blockchain with PoW and PoS ===" << std::endl;
    bc.displayChain();
    std::cout << "Chain Valid: " << (bc.isChainValid() ? "Yes" : "No") << std::endl;

    return 0;
}