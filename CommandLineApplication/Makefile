CXX = g++
CXXFLAGS = -std=c++17 -Wall -I../ThreadLibrary
LDFLAGS = -lpthread
LIB_DIR = ../ThreadLibrary
SRCS = $(LIB_DIR)/test.cpp $(LIB_DIR)/thread.cpp
TARGET = thread_test_app
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)