BUILD_DIR = build
TARGET = $(BUILD_DIR)/solution
CXX = g++
CXXFLAGS = -O3 -std=c++20 -pthread
SRC = $(wildcard *.cpp)

prepare: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: prepare
	@./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)