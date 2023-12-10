CC = g++
FLAGS = -Wall -std=c++11 -Wextra -pedantic
BASES_FILES = main.cpp main.h
TARGET = main
SIMLIB = -lsimlib -lm

.PHONY: all run line clean

all: $(TARGET)

$(TARGET): $(BASES_FILES)
	$(CC) $(FLAGS) -o $(TARGET) $(BASES_FILES) $(SIMLIB)

run: all
	./$(TARGET) -i 2 -n 2 -b
#	./$(TARGET) -i 2 -n 4 -b

line: all
	./$(TARGET) -i 2 -n 4 -d -l 3 -r 1

clean:
	rm -f $(TARGET)