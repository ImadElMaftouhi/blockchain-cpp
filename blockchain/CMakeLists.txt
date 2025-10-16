cmake_minimum_required(VERSION 3.10)
# project(blockchain-cpp VERSION 1.0 DATE "2025-10-16")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)    

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")  # OpenSSL
find_package(OpenSSL REQUIRED)

add_executable(blockchain
    src/merkle_tree.cpp
)

