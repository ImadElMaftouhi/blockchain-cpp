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

## 🔍 Key Concepts

### Merkle Tree
A hash tree that allows efficient verification of large data sets. Each leaf node contains a hash of a transaction, and each non-leaf node contains a hash of its children.

### Proof of Work
A consensus mechanism requiring miners to solve computationally intensive puzzles to add blocks. Ensures security through computational effort.

### Proof of Stake
An alternative consensus where validators are chosen based on their stake in the network, offering energy efficiency and faster validation.

### Blockchain Integrity
Each block contains the hash of the previous block, creating an immutable chain where tampering with any block invalidates all subsequent blocks.


## 🏗️ Project Structure

```
blockchain/
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
│   ├── validator.h              # Validator class
│   ├── pow.h                    # PoW mechanism
│   └── pos.h                    # PoS mechanism
│
├── src/
│   ├── utils.cpp
│   ├── merkle_tree.cpp
│   ├── transaction.cpp
│   ├── block.cpp                # Generic Block implementation
│   ├── block_pow.cpp
│   ├── block_pos.cpp
│   ├── blockchain.cpp           # Generic Blockchain implementation
│   ├── blockchain_pow.cpp
│   ├── blockchain_pos.cpp
│   ├── validator.cpp
│   ├── pow.cpp        # PoW implementation
│   ├── pos.cpp       # PoS implementation
│   └── main.cpp                 # Main entry point and demo
│
├── tests/
│   ├── test_ex1_merkle.cpp      # Test for Exercise 1
│   ├── test_ex2_pow.cpp         # Test for Exercise 2
│   ├── test_ex3_pos.cpp         # Test for Exercise 3 (PoW vs PoS)
│   └── test_ex4_complete.cpp    # Test for Exercise 4 (complete integration)
│
├── CMakeLists.txt               # Build configuration
├── Makefile                     # Alternative build system
├── LICENSE
└── README.md
```

### What you'll find here

- `include/` — public hea ders (utils, blocks, blockchain, consensus helpers)
- `src/` — implementations for the components and `main.cpp` demo
- `tests/` — small example/tests/exercises used by the demo
- `CMakeLists.txt` — CMake build script (targets: `blockchain_project` and per-exercise tests)

## Dependencies

- CMake 3.10+
- A C++ compiler (GCC or Clang; MinGW-w64 on Windows recommended)
- OpenSSL development libraries and headers (used for SHA-256)

On Windows, the repository was developed/tested with MSYS2/MinGW-w64. Install packages using the MSYS2 pacman tool if you use that environment:


```powershell
# for MSYS2 MinGW64
# in MSYS2 shell (mingw64)
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-openssl
```

If you don't use MSYS2, install OpenSSL dev package via your OS package manager (e.g., `apt install libssl-dev` on Debian/Ubuntu).

## Build (recommended)

Clone this repository with:

```
git clone https://github.com/ImadMaftouhi/blockchain-cpp.git
```

From the `blockchain` folder run:

```powershell
# Create an out-of-source build directory
cmake -S . -B build -G "Ninja"
cmake --build build
```

The build produces `build\blockchain_project.exe` and test executables in `build`.

## Run the demo

From the `blockchain` folder run:

```powershell
.\build\blockchain_project.exe
```

The program runs four exercises (Merkle Tree, PoW demo, PoS demo and complete integration) and prints results including timestamps and timings.

## Tests

There are small example/test programs in `tests/`. After building you can run them from the `build` directory:

```powershell
.\test_ex1_merkle.exe
.\test_ex2_pow.exe
.\test_ex3_pos.exe
.\test_ex4_complete.exe
```

## Notes & troubleshooting

- OpenSSL headers not found: ensure the OpenSSL development package is installed and set `OPENSSL_ROOT_DIR` in `CMakeLists.txt` if necessary. For MSYS2 MinGW64, headers live in `C:/msys64/mingw64/include` and libs in `C:/msys64/mingw64/lib`.
- `measureTime()` now returns microseconds (high-resolution). If you display timings as milliseconds in your prints, convert with the helper `microsToMillis()` in `include/utils.h` to avoid labeling microseconds as ms.
- Quick fixes: increase PoW difficulty in `main.cpp` to produce visible mining times when demonstrating performance differences.


## 📊 Performance Benchmarks

### Proof of Work vs Proof of Stake

| Consensus | Difficulty | Time (avg) | Energy |
|-----------|------------|------------|--------|
| PoW       | 2 zeros    | ~0.01s     | High   |
| PoW       | 4 zeros    | ~0.5s      | High   |
| PoW       | 6 zeros    | ~15s       | High   |
| PoS       | N/A        | ~0.001s    | Low    |

*Note: Times may vary based on hardware and implementation details*

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

## 👥 Authors

- **Imad El Maftouhi** - *Initial implementation* - [ImadElMaftouhi](https://github.com/ImadElMaftouhi)

## 🙏 Acknowledgments

- Pr. Ikram BENABDELOUAHAB for course instruction
- Master IASD program, 2025/2026

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 📚 References

- [Bitcoin Whitepaper](https://bitcoin.org/bitcoin.pdf)
- [Merkle Trees Explained](https://en.wikipedia.org/wiki/Merkle_tree)
- [Proof of Work vs Proof of Stake](https://ethereum.org/en/developers/docs/consensus-mechanisms/)

---

**Note**: This is an educational implementation for learning purposes. It is not intended for production use or handling real cryptocurrency transactions.

## TODO
- Add a CTest integration in `CMakeLists.txt` to run the test executables via `ctest`.
- Add CLI flags to `main.cpp` to control difficulty, number of transactions, and number of iterations for timing.
- Add unit tests for core components (Merkle hashing, block validity checks).
