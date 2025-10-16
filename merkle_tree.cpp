// merkle_tree.cpp
// Imad El Maftouhi

/* DEFINITION
Merkle tree is a binary tree where each internal node is the hash of its children nodes. It's structure help to summarize large amount of data (transactions) into a single hash value.
By combining the hashes of the transactions into a single root hash, the Merkle tree provides a way to verify the integrity of the data by only checking the root hash instead of all the individual transactions.

- What we need : 
1. Hash function, e.g. SHA256
2. Leaf Nodes 
3. Internal/Parent nodes
4. Root node

To Build the tree, I'm going to use a tree structure with Node Objects containing left/right pointers instead of a vector to store all nodes.

Our Node class will contain a hash value and a pointer to the left and right child nodes.

The main function I built into these are function that build the tree from data, get the root hash and verify if data exists in the tree. These will be used to validate the integrity of the data.


**Notes**
- The time complexity of building a Merkle Tree with N Transactions is O(N).
- In case we change one transaction after building the tree, we need to recompute the root hash to ensure the integrity of the data.
- TO verify that a specific transaction exists in the tree without rebuilding everthing, we need the hash value of the sibling, the parent node and the root node. Then we can compare the hashes to see if the transaction exists in the tree.
*/



#include <iostream>
#include <vector>
#include <string>
// #include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <chrono>


std::string sha256(const std::string& input) {
    // 1. Create array to hold the hash result (32 bytes for SHA-256)
    unsigned char hash[SHA256_DIGEST_LENGTH];  // DIGEST_LENGTH = 32
    
    // 2. Calculate the SHA-256 hash of the input string
    //    Takes: pointer to input data, length of input, where to store result
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    
    // 3. Now 'hash' contains 32 bytes like: [0xA3, 0xF5, 0x2B, ...]
    //    We need to convert this to a readable string
    
    std::stringstream ss;  // String builder
    
    // 4. Loop through each byte and convert to hexadecimal
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex                    // Use hexadecimal format
           << std::setw(2)                // Each byte = 2 hex digits
           << std::setfill('0')           // Pad with 0 if needed
           << (int)hash[i];               // Convert byte to int
    }
    
    // 5. Return the hex string like "a3f52b..."
    return ss.str();
}


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
        ~MerkleNode(){
            delete left;
            delete right;
        }
};

class MerkleTree {
    private:
        MerkleNode* root;

        MerkleNode* buildTreeRecursive(std::vector<MerkleNode*>& nodes) {
            //base case
            if (nodes.size() == 1){
                return nodes[0]; 
            }
            std::vector<MerkleNode*> parentNodes;
            for (size_t i=0; i<nodes.size();i+=2) {
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
        MerkleTree(const std::vector<std::string>& transactions){
            if (transactions.empty()){
                root = nullptr;
                return
            }

            std::vector<MerkleNode*> leafNodes;
            for (const auto& tx:transactions) {
                leafNodes.push_back(new MerkleNode(tx));
            }
            root = buildTreeRecursive(leafNodes);
        }

        /**
         * Returns the root hash of the Merkle tree.
         * If the tree is empty, returns an empty string.
         * @return The root hash of the Merkle tree.
         */
        std::string getRootHash() const {
            if (root == nullptr){
                return "";
            }
            return root->hash;
        }

        void printTree(MerkleNode* node, int depth=0) const {
            if (node==nullptr) return;
            for (int i=0, i<depth, i++){
                std::cout << "  ";
            }
            std::cout << "Hash: " << node->hash.substr(0, 16) << "..." std:endl;

            if (node->left !=nullptr || node->right !=nullptr ){
                printTree(node->left, depth+1);
                printTree(node->right, depth+1);
            }
        }

        void printTree() const {
            std::cout << "\n=== Merke Tree Structure" << std::endl;
            printTree(root);
            std::cout << "========================\n" << std::endl;
        }

        bool verifyTransaction(const std::string& transaction) const {
            std::string txHash = sha256(transaction);
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


// MAIN: DEMONSTRATION AND TESTING
int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "   MERKLE TREE IMPLEMENTATION DEMO" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // ========== TEST 1: Basic Tree with 4 Transactions ==========
    std::cout << "TEST 1: Building Merkle Tree with 4 transactions" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    
    std::vector<std::string> transactions1 = {
        "Alice sends 50 BTC to Bob",
        "Bob sends 30 BTC to Charlie",
        "Charlie sends 20 BTC to David",
        "David sends 10 BTC to Eve"
    };
    
    auto start = std::chrono::high_resolution_clock::now();
    MerkleTree tree1(transactions1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Transactions:" << std::endl;
    for (size_t i = 0; i < transactions1.size(); i++) {
        std::cout << "  " << i+1 << ". " << transactions1[i] << std::endl;
    }
    std::cout << "\nMerkle Root: " << tree1.getRootHash() << std::endl;
    std::cout << "Build Time: " << duration.count() << " microseconds" << std::endl;
    
    tree1.printTree();

    // ========== TEST 2: Odd Number of Transactions ==========
    std::cout << "\nTEST 2: Building Merkle Tree with 3 transactions (odd number)" << std::endl;
    std::cout << "--------------------------------------------------------------" << std::endl;
    
    std::vector<std::string> transactions2 = {
        "Transaction A",
        "Transaction B",
        "Transaction C"
    };
    
    MerkleTree tree2(transactions2);
    std::cout << "Transactions:" << std::endl;
    for (size_t i = 0; i < transactions2.size(); i++) {
        std::cout << "  " << i+1 << ". " << transactions2[i] << std::endl;
    }
    std::cout << "\nMerkle Root: " << tree2.getRootHash() << std::endl;
    
    tree2.printTree();

    // ========== TEST 3: Single Transaction ==========
    std::cout << "\nTEST 3: Building Merkle Tree with 1 transaction" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    
    std::vector<std::string> transactions3 = {
        "Single Transaction"
    };
    
    MerkleTree tree3(transactions3);
    std::cout << "Transaction: " << transactions3[0] << std::endl;
    std::cout << "Merkle Root: " << tree3.getRootHash() << std::endl;
    
    tree3.printTree();

    // ========== TEST 4: Verification ==========
    std::cout << "\nTEST 4: Transaction Verification" << std::endl;
    std::cout << "---------------------------------" << std::endl;
    
    std::string existingTx = "Alice sends 50 BTC to Bob";
    std::string nonExistingTx = "Eve sends 100 BTC to Frank";
    
    std::cout << "Verifying existing transaction: \"" << existingTx << "\"" << std::endl;
    std::cout << "Result: " << (tree1.verifyTransaction(existingTx) ? "FOUND ✓" : "NOT FOUND ✗") << std::endl;
    
    std::cout << "\nVerifying non-existing transaction: \"" << nonExistingTx << "\"" << std::endl;
    std::cout << "Result: " << (tree1.verifyTransaction(nonExistingTx) ? "FOUND ✓" : "NOT FOUND ✗") << std::endl;
    
    // ========== TEST 5: Large Dataset ==========
    std::cout << "\nTEST 5: Performance with Large Dataset" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    
    std::vector<std::string> largeDataset;
    for (int i = 0; i < 1000; i++) {
        largeDataset.push_back("Transaction " + std::to_string(i));
    }
    
    start = std::chrono::high_resolution_clock::now();
    MerkleTree largeTree(largeDataset);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Number of transactions: " << largeDataset.size() << std::endl;
    std::cout << "Merkle Root: " << largeTree.getRootHash().substr(0, 32) << "..." << std::endl;
    std::cout << "Build Time: " << duration.count() << " microseconds" << std::endl;





    // ========== TEST 6: Data Integrity ==========
    std::cout << "\nTEST 6: Data Integrity Check" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    
    std::vector<std::string> originalData = {
        "Transaction 1",
        "Transaction 2",
        "Transaction 3",
        "Transaction 4"
    };
    
    MerkleTree originalTree(originalData);
    std::string originalRoot = originalTree.getRootHash();
    
    std::cout << "Original Merkle Root: " << originalRoot.substr(0, 32) << "..." << std::endl;
    
    return 0;
}
