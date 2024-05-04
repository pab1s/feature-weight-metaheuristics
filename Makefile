CXX = g++
CXXFLAGS = -Wall -g -Iinclude -I/usr/src/googletest/googletest/include -fopenmp
LDFLAGS = -fopenmp

# Profile and Debug options
ifeq ($(PROFILE),yes)
    CXXFLAGS += -pg
    LDFLAGS += -pg
endif

ifeq ($(DEBUG),yes)
    CXXFLAGS += -Og -DDEBUG
else
    CXXFLAGS += -O3
endif

# Targets
TARGET = bin/metaheuristics
TEST_TARGET = bin/runTests

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TESTS_DIR = tests
DOC_DIR = docs

# Create necessary directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)/algorithms $(OBJ_DIR)/data $(OBJ_DIR)/utils $(DOC_DIR))

# Source files and objects
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/algorithms/*.cpp) \
       $(wildcard $(SRC_DIR)/data/*.cpp) \
       $(wildcard $(SRC_DIR)/utils/*.cpp)

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test source files and objects
TEST_SRCS = $(wildcard $(TESTS_DIR)/*.cpp)
TEST_OBJS = $(TEST_SRCS:$(TESTS_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Google Test libraries
GTEST_LIB = -L/usr/src/googletest/googletest/lib -lgtest -lgtest_main -pthread

# Doxygen configuration
DOXYGEN_CONFIG = config.doxy

# Main build rule
all: $(TARGET)

# Link the main executable
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/algorithms/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/data/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/utils/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Test build and execution
test: $(OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) $(filter-out $(OBJ_DIR)/main.o, $(OBJS)) $(GTEST_LIB) -o $(TEST_TARGET)
	./$(TEST_TARGET)

# Generate documentation using Doxygen
docs: $(DOXYGEN_CONFIG)
	doxygen $(DOXYGEN_CONFIG)

# Clean up build files
clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJ_DIR)/algorithms/*.o $(OBJ_DIR)/data/*.o $(OBJ_DIR)/utils/*.o $(BIN_DIR)/*

# Clean up documentation
clean-docs:
	rm -rf $(DOC_DIR)

# Phony targets
.PHONY: all clean test docs clean-docs
