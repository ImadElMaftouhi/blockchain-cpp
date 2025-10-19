#include "merkle_tree.h"
#include <vector>
#include <iostream>
#include <cassert>

int main() {
    std::vector<std::string> transactions = {
        "Tx1: Alice->Bob:50", 
        "Tx2: Bob->Charlie:30", 
        "Tx3: Charlie->David:20"
    };
    MerkleTree merkleTree(transactions);
    assert(!merkleTree.getRootHash().empty());
    merkleTree.printTree();
    std::cout << "Merkle Tree Test Passed: Root Hash generated and tree printed.\n";
    assert(merkleTree.verifyTransaction("Tx1: Alice->Bob:50"));
    std::cout << "Merkle Tree Test Passed: Transaction verification works.\n";
    return 0;
}