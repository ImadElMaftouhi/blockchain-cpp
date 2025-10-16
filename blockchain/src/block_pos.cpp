#include "block_pos.h"
#include "utils.h"
#include "pos.h"
#include <iostream>
#include <sstream>

BlockPos::BlockPos(int idx, const std::string& prevHash, const std::string& h, const std::string& d, const std::string& v)
    : index(idx), previousHash(prevHash), hash(h), data(d), validator(v) {}

BlockPos::~BlockPos() {}

std::string BlockPos::getHash() const {
    return hash;
}

std::string BlockPos::getPreviousHash() const {
    return previousHash;
}

int BlockPos::getIndex() const {
    return index;
}

std::string BlockPos::calculateHash() const {
    std::stringstream ss;
    ss << index << getCurrentTime() << data << previousHash << validator;
    return sha256(ss.str());
}

void BlockPos::display() const {
    std::cout << "\n--- Block #" << index << " (PoS) ---" << std::endl;
    std::cout << "Timestamp: " << getCurrentTime() << std::endl;
    std::cout << "Data: " << data << std::endl;
    std::cout << "Validator: " << validator << std::endl;
    std::cout << "Previous Hash: " << previousHash.substr(0, 16) << "..." << std::endl;
    std::cout << "Hash: " << hash.substr(0, 16) << "..." << std::endl;
}

std::string BlockPos::getData() const {
    return data;
}

std::string BlockPos::getValidator() const {
    return validator;
}