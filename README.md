# Blockchain Project

This project implements a simplified blockchain system in C++ as part of the "ATELIER 1 : IMPLEMENTATION COMPLETE D’UNE BLOCKCHAIN SIMPLIFIEE EN C++" homework for the 2ème année Master IASD, 2025/2026 module taught by Pr. Ikram BENABDELOUAHAB. The implementation covers key blockchain concepts and provides a modular, testable structure to demonstrate Proof of Work (PoW), Proof of Stake (PoS), and Merkle Tree functionalities.

## Project Overview

The blockchain system is designed to explore and compare different consensus mechanisms and data structures used in blockchain technology. It includes:

- **Merkle Tree**: A binary tree structure used to efficiently summarize and verify a set of transactions, ensuring data integrity.
- **Proof of Work (PoW)**: A consensus mechanism where miners solve computationally intensive puzzles to validate blocks, securing the chain with adjustable difficulty levels.
- **Proof of Stake (PoS)**: An alternative consensus mechanism where validators are chosen based on their stake, offering a faster and less resource-intensive validation process.
- **Blockchain Implementation**: A modular design with generic and specific implementations for PoW and PoS, including block and chain management.

## Directory Structure

- `include/`: Header files defining classes and interfaces (e.g., `utils.h`, `merkle_tree.h`, `blockchain_pow.h`).
- `src/`: Source files with implementations (e.g., `main.cpp`, `block_pow