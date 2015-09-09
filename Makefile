CC=clang++
FLAG=-Wall -std=c++11 -g -O0 -pthread
MAIN=main
TEST=test

.PHONY: all test clean

all: main test

logger.o:
	$(CC) $(FLAG) -c ./logging/logger.cpp

http_serv.o:
	$(CC) $(FLAG) -c http_serv.cpp

tcp_socket.o:
	$(CC) $(FLAG) -c tcp_socket.cpp

httpd.o:
	$(CC) $(FLAG) -c httpd.cpp

main.o:
	$(CC) $(FLAG) -c main.cpp

main: main.o logger.o http_serv.o tcp_socket.o httpd.o
	$(CC) $(FLAG) http_serv.o httpd.o tcp_socket.o logger.o main.o -lm -o $(MAIN)

clean:
	rm *.o $(MAIN)
