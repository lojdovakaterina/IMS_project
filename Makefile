CC = g++
FLAGS = -Wall -std=c++11 -Wextra -pedantic
BASES_FILES = main.cpp main.h
TARGET = main
SIMLIB = -lsimlib -lm

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(BASES_FILES)
	$(CC) $(FLAGS) -o $(TARGET) $(BASES_FILES) $(SIMLIB)

run: all
	./$(TARGET) -i 3 -n 3 -b

clean:
	rm -f $(TARGET)