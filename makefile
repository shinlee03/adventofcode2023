# USE AS TEMPLATE :) 

CXX=g++
CXXFLAGS = -std=c++20 -Wall -Werror -g -O3
TARGET = main
COMPARE = compare

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(TARGET)

$(COMPARE): compare.cpp
	$(CXX) $(CXXFLAGS) compare.cpp -o $(COMPARE)

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