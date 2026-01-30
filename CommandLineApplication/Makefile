CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread -I../ThreadLibrary -I.
LDFLAGS = -pthread

THREAD_LIB_DIR = ../ThreadLibrary
BUILD_DIR = build

THREAD_LIB_SRCS = $(THREAD_LIB_DIR)/thread.cpp
APP_SRCS = main.cpp counter.cpp global_counter.cpp thread_scheduler.cpp

THREAD_LIB_OBJS = $(BUILD_DIR)/thread.o
APP_OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/counter.o $(BUILD_DIR)/global_counter.o $(BUILD_DIR)/thread_scheduler.o

TARGET = counter_app

.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/thread.o: $(THREAD_LIB_DIR)/thread.cpp $(THREAD_LIB_DIR)/thread.h $(THREAD_LIB_DIR)/runnable.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/counter.o: counter.cpp counter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/global_counter.o: global_counter.cpp global_counter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/thread_scheduler.o: thread_scheduler.cpp thread_scheduler.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(THREAD_LIB_OBJS) $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)