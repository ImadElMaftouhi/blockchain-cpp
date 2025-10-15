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


#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>


std::string sha256(const std::string& input){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    std::stringstream ss;
    for(int i=0, i<SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std:setw(2) << std::setfill("0") << (int)hash[i];
    }
    return ss.str();
}


class MerkleNode {
    public:
        std::string hash;
        MerkleNode* left;
        MerkleNode* right;

        MerkleNode(const std::string& data) {
            // what should this constructor do with the data ?
        };

        MerkleNode(MerkleNode* l, MerkleNode* r) {
            // What should this constructor do with the two child nodes ?
        };

        // Destructor
        ~MerkleNode() {
            // Implementation here
        }
};
