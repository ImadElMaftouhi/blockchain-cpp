CC = g++
CFLAGS = -Wall -g -Iinclude
LDFLAGS = -lssl -lcrypto

SOURCES = src/main.cpp src/utils.cpp src/merkle_tree.cpp src/transaction.cpp \
          src/block.cpp src/block_pow.cpp src/block_pos.cpp src/blockchain.cpp \
          src/blockchain_pow.cpp src/blockchain_pos.cpp src/validator.cpp \
          src/proof_of_work.cpp src/proof_of_stake.cpp

TESTS = tests/test_ex1_merkle.cpp tests/test_ex2_pow.cpp tests/test_ex3_pos.cpp tests/test_ex4_complete.cpp

OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TESTS:.cpp=.o)

TARGET = blockchain_project
TEST_TARGETS = test_ex1_merkle test_ex2_pow test_ex3_pos test_ex4_complete

all: $(TARGET) $(TEST_TARGETS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(TEST_TARGETS): $(TEST_OBJECTS) $(OBJECTS)
	$(CC) $(filter %$@.o, $(TEST_OBJECTS)) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TEST_OBJECTS) $(TARGET) $(TEST_TARGETS)

.PHONY: all clean