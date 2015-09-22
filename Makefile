CC = clang++
CFLAGS = -Wall -std=c++11 -g -O0 -pthread
SRCS = $(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
MAIN = main
TEST = test

.PHONY: all test clean

all: main

logger.o:
	$(CC) $(CFLAGS) -c ./logging/logger.cpp

$(OBJS):
	$(CC) $(CFLAGS) -c $(addsuffix .cpp, $(basename $@))

main: $(OBJS) logger.o
	$(CC) $(CFLAGS) $(OBJS) logger.o -lm -o $(MAIN)

clean:
	rm *.o $(MAIN)
