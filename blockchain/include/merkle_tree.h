#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <string>
#include <vector>

class MerkleNode {
public:
    std::string hash;
    MerkleNode* left;
    MerkleNode* right;

    MerkleNode(const std::string& data);
    MerkleNode(MerkleNode* l, MerkleNode* r);
    ~MerkleNode();
};

class MerkleTree {
private:
    MerkleNode* root;

    MerkleNode* buildTreeRecursive(std::vector<MerkleNode*>& nodes);
    bool searchHash(MerkleNode* node, const std::string& targetHash) const;

public:
    MerkleTree(const std::vector<std::string>& transactions);
    std::string getRootHash() const;
    void printTree(MerkleNode* node, int depth = 0) const;
    void printTree() const;
    bool verifyTransaction(const std::string& transaction) const;

    ~MerkleTree();
};

#endif
