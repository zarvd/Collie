CC = clang++
FLAG = -Wall -std=c++11 -g -O0 -pthread
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HTTP_SRCS = $(wildcard ./http/*.cpp)
HTTP_OBJS = $(HTTP_SRCS:.cpp=.o)
MAIN = main
TEST = test

.PHONY: all test clean

all: main

Logger.o:
	make -C logging prod
	cp logging/prod/Logger.o ./

$(OBJS):
	$(CC) $(FLAG) -c $(addsuffix .cpp, $(basename $@))

$(HTTP_OBJS):
	$(CC) $(FLAG) -c $(addsuffix .cpp, $(basename $@)) -o $@

Http.o: $(HTTP_OBJS)
	ld -r $(HTTP_OBJS) -o ./Http.o

main: $(OBJS) Http.o Logger.o
	$(CC) $(FLAG) $(OBJS) Http.o Logger.o -lm -o $(MAIN)

clean:
	make -C logging clean
	-rm *.o $(MAIN)
	-rm http/*.o
