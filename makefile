CXX = g++
CXXFLAGS = -I./include -Wall -std=c++11
LDFLAGS = -L./lib -lmingw32 -lSDL2main -lSDL2

SRCS = $(wildcard src/*.cpp)
OBJS = $(patsubst src/%.cpp, obj/%.o, $(SRCS))
TARGET = maze

ifeq ($(OS),Windows_NT)
    RM = cmd /C del /Q
    MKDIR = cmd /C mkdir
else
    RM = rm -f
    MKDIR = mkdir -p
endif

all: create_objdir $(TARGET)

create_objdir:
	@if exist obj rmdir /S /Q obj
	$(MKDIR) obj

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)

.PHONY: all clean create_objdir
