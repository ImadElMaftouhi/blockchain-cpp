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
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>


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

    
};
