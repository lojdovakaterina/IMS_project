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
	./$(TARGET) -i 2 -n 2 -b
#	./$(TARGET) -i 2 -n 4 -b
# 	./$(TARGET) -i 3 -n 4 -argument
#	./$(TARGET) -i 3 -n 4 -b -b

line: all
	./$(TARGET) -i 2 -n 4 -l "3 1"

clean:
	rm -f $(TARGET)