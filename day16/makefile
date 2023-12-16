CXX=g++
CXXFLAGS = -std=c++20 -Wall -Werror
TARGET = main

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

test1: $(TARGET)
	./$(TARGET) example.txt 1

task1: $(TARGET)
	./$(TARGET) input.txt 1

test2: $(TARGET) 
	./$(TARGET) example.txt 2

task2: $(TARGET)
	./$(TARGET) input.txt 2

clean: 
	rm -f $(TARGET)

.PHONY: all run clean