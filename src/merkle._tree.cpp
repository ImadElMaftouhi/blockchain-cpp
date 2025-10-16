#include "merkle_tree.h"
#include <iostream>
#include <sstream>
#include <iomanip>

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.length(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

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
        MerkleNode* right = (i + 1 < nodes.size()) ? nodes[i + 1] : left;
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

void MerkleTree::printTree() const {
    std::cout << "Merkle Root: " << getRootHash() << std::endl;
}

bool MerkleTree::verifyTransaction(const std::string& transaction) const {
    std::string txHash = sha256(transaction);
    // Simple check for now, expand later
    return root && root->hash == txHash;
}

MerkleTree::~MerkleTree() {
    delete root;
}