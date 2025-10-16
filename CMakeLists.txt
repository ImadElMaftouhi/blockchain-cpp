cmake_minimum_required(VERSION 3.10)
project(blockchain-cpp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
find_package(OpenSSL REQUIRED)

# Add source files
add_executable(blockchain
    src/main.cpp
    src/merkle_tree.cpp
)

# Link OpenSSL libraries
target_link_libraries(blockchain OpenSSL::SSL OpenSSL::Crypto)