CXX = g++
CXXFLAGS = -std=c++17 -O2 -pthread

TARGET = server

SRC = Day5.cpp

LIBS = -lcurl

all:
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LIBS)

run:
	./server