CC=clang++
FLAG=-Wall -std=c++11 -g -O0 -pthread
MAIN=main
TEST=test

.PHONY: all test clean

all: test

logger.o:
	$(CC) $(FLAG) -c ./logging/logger.cpp

clean:
	rm *.o $(MAIN) $(TEST)
