CXX = g++
CXXFLAGS = -Wall

TARGET = chatgpt_evm 
SRC = main.cpp EVM.cpp Memory.cpp uint256.cpp Stack.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
