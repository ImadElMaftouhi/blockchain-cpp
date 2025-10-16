#  Blockchain Implementation in C++

A complete educational implementation of a simplified blockchain system from scratch, demonstrating core blockchain concepts including Merkle Trees, Proof of Work, and Proof of Stake consensus mechanisms.

## 📋 Project Overview

This project is part of the Blockchain module for the 2nd year Master IASD program (2025/2026), instructed by Pr. Ikram BENABDELOUAHAB. It provides a comprehensive, ground-up implementation of fundamental blockchain components in C++.

## 🎯 Features

### 1. Merkle Tree
- Implementation of a basic Merkle Tree data structure
- Efficient transaction summarization
- Hash-based verification of data integrity
- Root hash calculation for transaction sets

### 2. Proof of Work (PoW)
- Mining algorithm with adjustable difficulty
- Nonce-based hash calculation
- Performance benchmarking across difficulty levels
- Computational puzzle solving for block validation

### 3. Proof of Stake (PoS)
- Stake-based validator selection
- Weighted random validator assignment
- Energy-efficient alternative to PoW
- Comparative performance analysis

### 4. Complete Blockchain System
- Transaction management (sender, receiver, amount)
- Block structure with timestamps and linking
- Chain integrity verification
- Support for both PoW and PoS consensus
- Performance comparison and analytics

## 🏗️ Project Structure

```
blockchain-cpp/
├── include/
│   ├── utils.h                  # Shared utilities (SHA-256, time)
│   ├── merkle_tree.h            # Merkle Tree
│   ├── transaction.h            # Transaction class
│   ├── block.h                  # Generic Block interface
│   ├── block_pow.h              # PoW Block
│   ├── block_pos.h              # PoS Block
│   ├── blockchain.h             # Generic Blockchain interface
│   ├── blockchain_pow.h         # PoW Blockchain
│   ├── blockchain_pos.h         # PoS Blockchain
│   └── validator.h              # Validator class
│
├── src/
│   ├── utils.cpp
│   ├── merkle_tree.cpp
│   ├── transaction.cpp
│   ├── block_pow.cpp
│   ├── block_pos.cpp
│   ├── blockchain_pow.cpp
│   ├── blockchain_pos.cpp
│   └── validator.cpp
│
├── tests/
│   ├── test_ex1_merkle.cpp      
│   ├── test_ex2_pow.cpp         
│   ├── test_ex3_pos.cpp         # PoW vs PoS
│   └── test_ex4_complete.cpp    # Complete integration
│
├── CMakeLists.txt               # Build configuration
├── Makefile                     # Alternative build system
└── README.md
```

## 🚀 Getting Started

### Prerequisites
- C++ compiler with C++17 support (GCC 7+ or Clang 5+)
- CMake 3.10 or higher
- OpenSSL library (for SHA-256 hashing)

### Installation

```bash
# Clone the repository
git clone https://github.com/ImadElMaftouhi/blockchain-cpp.git
cd blockchain-cpp

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the demo
./blockchain_demo
```

### Quick Example

```cpp
#include "blockchain.h"
#include "transaction.h"

int main() {
    // Create a new blockchain
    Blockchain blockchain;
    
    // Create transactions
    Transaction tx1("Alice", "Bob", 50);
    Transaction tx2("Bob", "Charlie", 30);
    
    // Add block with Proof of Work
    std::vector<Transaction> transactions = {tx1, tx2};
    blockchain.addBlockPoW(transactions, 4); // difficulty = 4
    
    // Verify blockchain integrity
    if (blockchain.isValid()) {
        std::cout << "Blockchain is valid!" << std::endl;
    }
    
    return 0;
}
```

## 📊 Performance Benchmarks

### Proof of Work vs Proof of Stake

| Consensus | Difficulty | Time (avg) | Energy |
|-----------|------------|------------|--------|
| PoW       | 2 zeros    | ~0.01s     | High   |
| PoW       | 4 zeros    | ~0.5s      | High   |
| PoW       | 6 zeros    | ~15s       | High   |
| PoS       | N/A        | ~0.001s    | Low    |

*Note: Times may vary based on hardware and implementation details*

## 🔍 Key Concepts Demonstrated

### Merkle Tree
A hash tree that allows efficient verification of large data sets. Each leaf node contains a hash of a transaction, and each non-leaf node contains a hash of its children.

### Proof of Work
A consensus mechanism requiring miners to solve computationally intensive puzzles to add blocks. Ensures security through computational effort.

### Proof of Stake
An alternative consensus where validators are chosen based on their stake in the network, offering energy efficiency and faster validation.

### Blockchain Integrity
Each block contains the hash of the previous block, creating an immutable chain where tampering with any block invalidates all subsequent blocks.

## 🧪 Testing

```bash
# Run all tests
cd build
./run_tests

# Test specific components
./test_merkle_tree
./test_pow
./test_pos
```

## 📈 Exercises Implemented

- ✅ **Exercise 1**: Merkle Tree implementation with execution examples
- ✅ **Exercise 2**: Proof of Work with variable difficulty and timing analysis
- ✅ **Exercise 3**: Proof of Stake with comparative performance analysis
- ✅ **Exercise 4**: Complete blockchain integration with:
  - Transaction and block management
  - PoW mining with difficulty adjustment
  - PoS validation mechanism
  - Comparative analysis of both consensus mechanisms

## 🤝 Contributing

This is an educational project. Suggestions and improvements are welcome!

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/improvement`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/improvement`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👥 Authors

- **Imad El Maftouhi** - *Initial implementation* - [ImadElMaftouhi](https://github.com/ImadElMaftouhi)

## 🙏 Acknowledgments

- Pr. Ikram BENABDELOUAHAB for course instruction
- Master IASD program, 2025/2026

## 📚 References

- [Bitcoin Whitepaper](https://bitcoin.org/bitcoin.pdf)
- [Merkle Trees Explained](https://en.wikipedia.org/wiki/Merkle_tree)
- [Proof of Work vs Proof of Stake](https://ethereum.org/en/developers/docs/consensus-mechanisms/)

---

**Note**: This is an educational implementation for learning purposes. It is not intended for production use or handling real cryptocurrency transactions.