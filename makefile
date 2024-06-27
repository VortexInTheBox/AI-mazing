# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -I./include -Wall -std=c++11

# Linker flags
LDFLAGS = -L./lib -lmingw32 -lSDL2main -lSDL2

# Source files
SRCS = $(wildcard src/*.cpp)

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = maze

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)