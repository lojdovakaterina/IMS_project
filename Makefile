CC = g++
FLAGS = -Wall -std=c++11
SRC = main.cpp
TARGET = main
SIMLIB = -lsimlib -lm

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(FLAGS) -o $(TARGET) $(SRC) $(SIMLIB)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)