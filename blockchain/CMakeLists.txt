cmake_minimum_required(VERSION 3.10)
# project(blockchain-cpp VERSION 1.0 DATE "2025-10-16")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")  # OpenSSL
find_package(OpenSSL REQUIRED)

add_executable(blockchain
    src/main.cpp
    src/merkle_tree.cpp
    src/pow.cpp
    src/pos.cpp
    src/blockchain.cpp
)

target_link_libraries(blockchain OpenSSL::SSL OpenSSL::Crypto)

enable_testing()
add_executable(test_merkle tests/test_merkle.cpp src/merkle_tree.cpp)
target_link_libraries(test_merkle OpenSSL::SSL OpenSSL::Crypto)
add_test(NAME MerkleTest COMMAND test_merkle)

add_executable(test_pow tests/test_pow.cpp src/pow.cpp)
target_link_libraries(test_pow OpenSSL::SSL OpenSSL::Crypto)
add_test(NAME PoWTest COMMAND test_pow)

add_executable(test_pos tests/test_pos.cpp src/pos.cpp)
target_link_libraries(test_pos OpenSSL::SSL OpenSSL::Crypto)
add_test(NAME PoSTest COMMAND test_pos)