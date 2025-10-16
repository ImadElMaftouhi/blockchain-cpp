#include "merkle_tree.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <iomanip>

MerkleNode::MerkleNode(const std::string& data) {
    this->hash = sha256(data);
    this->left = nullptr;
    this->right = nullptr;
}

MerkleNode::MerkleNode(MerkleNode* l, MerkleNode* r) {
    this->left = l;
    this->right = r;
    std::string combined = l->hash + r->hash;
    this->hash = sha256(combined);
}

MerkleNode::~MerkleNode() {
    delete left;
    delete right;
}

MerkleTree::MerkleTree(const std::vector<std::string>& transactions) {
    if (transactions.empty()) {
        root = nullptr;
        return;
    }

    std::vector<MerkleNode*> leafNodes;
    for (const auto& tx : transactions) {
        leafNodes.push_back(new MerkleNode(tx));
    }
    root = buildTreeRecursive(leafNodes);
}

MerkleNode* MerkleTree::buildTreeRecursive(std::vector<MerkleNode*>& nodes) {
    if (nodes.size() == 1) {
        return nodes[0];
    }
    std::vector<MerkleNode*> parentNodes;
    for (size_t i = 0; i < nodes.size(); i += 2) {
        MerkleNode* left = nodes[i];
        MerkleNode* right = (i + 1 == nodes.size()) ? nodes[i] : nodes[i + 1];
        MerkleNode* parent = new MerkleNode(left, right);
        parentNodes.push_back(parent);
    }
    return buildTreeRecursive(parentNodes);
}

std::string MerkleTree::getRootHash() const {
    if (root == nullptr) {
        return "";
    }
    return root->hash;
}

void MerkleTree::printTree(MerkleNode* node, int depth) const {
    if (node == nullptr) return;
    for (int i = 0; i < depth; i++) {
        std::cout << "  ";
    }
    std::cout << "Hash: " << node->hash.substr(0, 16) << "..." << std::endl;

    if (node->left != nullptr || node->right != nullptr) {
        printTree(node->left, depth + 1);
        printTree(node->right, depth + 1);
    }
}

void MerkleTree::printTree() const {
    std::cout << "\n=== Merkle Tree Structure" << std::endl;
    printTree(root);
    std::cout << "========================\n" << std::endl;
}

bool MerkleTree::verifyTransaction(const std::string& transaction) const {
    std::string txHash = sha256(transaction);
    return searchHash(root, txHash);
}

bool MerkleTree::searchHash(MerkleNode* node, const std::string& targetHash) const {
    if (node == nullptr) return false;

    if (node->hash == targetHash) return true;
    return searchHash(node->left, targetHash) || searchHash(node->right, targetHash);
}

MerkleTree::~MerkleTree() {
    delete root;
}