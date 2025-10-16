#include <iostream>
#include "merkle_tree.h"

int main() {
    std::vector<std::string> transactions = {
        "Alice sends 50 BTC to Bob",
        "Bob sends 30 BTC to Charlie"
    };
    MerkleTree tree(transactions);
    tree.printTree();
    std::cout << "Verify transaction: " << (tree.verifyTransaction("Alice sends 50 BTC to Bob") ? "Yes" : "No") << std::endl;
    return 0;
}