CC = g++
FLAGS = -Wall -std=c++11 -Wextra -pedantic
SRC = main.cpp
TARGET = main
SIMLIB = -lsimlib -lm

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(FLAGS) -o $(TARGET) $(SRC) $(SIMLIB)

run: all
	./$(TARGET) -i 3 -n 3 -b

clean:
	rm -f $(TARGET)