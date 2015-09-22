CC = clang++
FLAG = -Wall -std=c++11 -g -O0 -pthread
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
MAIN = main
TEST = test

.PHONY: all test clean

all: main

Logger.o:
	make -C logging prod
	cp logging/prod/Logger.o ./

$(OBJS):
	$(CC) $(FLAG) -c $(addsuffix .cpp, $(basename $@))

main: $(OBJS) Logger.o
	$(CC) $(FLAG) $(OBJS) Logger.o -lm -o $(MAIN)

clean:
	make -C logging clean
	-rm *.o $(MAIN)
