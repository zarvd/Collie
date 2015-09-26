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
	cd http
	$(CC) $(FLAG) -c $(addsuffix .cpp, $(basename $@))
	ld -r $(OBJS_WITH_NO_TEST) -o ./HTTP_LIB.o
	cd ..
	cp http/HTTP_LIB.o ./

main: $(OBJS) Logger.o
	$(CC) $(FLAG) $(OBJS) HTTP_LIB.o Logger.o -lm -o $(MAIN)

clean:
	make -C logging clean
	-rm *.o $(MAIN)
