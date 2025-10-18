# Compiler to use
CXX = g++

# Compiler flags: -std=c++17 for modern C++, -g for debugging, -Iinclude to find headers
CXXFLAGS = -std=c++17 -g -Iinclude

# The name of our final executable
TARGET = bin/game

# Find all .cpp files in the src directory
SRCS = $(wildcard src/*.cpp)

# Replace the .cpp extension with .o for object files
OBJS = $(SRCS:.cpp=.o)

# Default command, runs when you just type "make"
all: $(TARGET)

# Rule to link all object files into the final executable
$(TARGET): $(OBJS)
    @mkdir -p bin # Create the bin directory if it doesn't exist
    $(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
    @echo "Build successful! Run with ./bin/game"

# Rule to compile a .cpp file into a .o object file
%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# Command to clean up compiled files
clean:
    rm -f $(OBJS) $(TARGET)
    @echo "Cleanup complete."