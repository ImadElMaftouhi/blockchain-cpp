cmake_minimum_required(VERSION 3.10)
project(blockchain)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)    

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")  # OpenSSL
find_package(OpenSSL REQUIRED)

if (TARGET OpenSSL::SSL AND TARGET OpenSSL::Crypto)
    set(OPENSSL_LIBS OpenSSL::SSL OpenSSL::Crypto)
else()
    # Fallback to the older variables if imported targets are not available
    set(OPENSSL_LIBS ssl crypto)
    if (OPENSSL_INCLUDE_DIR)
        include_directories(${OPENSSL_INCLUDE_DIR})
    else()
        # Common MinGW path for OpenSSL headers
        include_directories(${OPENSSL_ROOT_DIR}/include)
    endif()
endif()

add_library(blockchain_lib STATIC
    src/utils.cpp
    src/merkle_tree.cpp
    src/transaction.cpp
    src/block.cpp
    src/block_pow.cpp
    src/block_pos.cpp
    src/blockchain.cpp
    src/blockchain_pow.cpp
    src/blockchain_pos.cpp
    src/validator.cpp
    src/pow.cpp
    src/pos.cpp
)
target_link_libraries(blockchain_lib PRIVATE ${OPENSSL_LIBS})

add_executable(blockchain_project src/main.cpp)
target_link_libraries(blockchain_project PRIVATE blockchain_lib ${OPENSSL_LIBS})

# Add test targets
add_executable(test_ex1_merkle tests/test_ex1_merkle.cpp)
target_link_libraries(test_ex1_merkle PRIVATE blockchain_lib ${OPENSSL_LIBS})

add_executable(test_ex2_pow tests/test_ex2_pow.cpp)
target_link_libraries(test_ex2_pow PRIVATE blockchain_lib ${OPENSSL_LIBS})

add_executable(test_ex3_pos tests/test_ex3_pos.cpp)
target_link_libraries(test_ex3_pos PRIVATE blockchain_lib ${OPENSSL_LIBS})

add_executable(test_ex4_complete tests/test_ex4_complete.cpp)
target_link_libraries(test_ex4_complete PRIVATE blockchain_lib ${OPENSSL_LIBS})