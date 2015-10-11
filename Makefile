CC = clang++
FLAG = -Wall -std=c++11 -g -O0 -pthread
BASE_SRCS = $(wildcard ./base/*cpp)
BASE_OBJS = $(BASE_SRCS:.cpp=.o)
HTTP_SRCS = $(wildcard ./http/*.cpp)
HTTP_OBJS = $(HTTP_SRCS:.cpp=.o)
BASE_TEST = BaseTest
HTTP_TEST = httpTest

.PHONY: all BaseTest httpTest clean

all: BaseTest httpTest

Logger.o:
	make -C logging prod
	cp logging/prod/Logger.o ./

Httpd.o:
	$(CC) $(FLAG) -c Httpd.cpp -o Httpd.o

$(HTTP_OBJS):
	$(CC) $(FLAG) -c $(addsuffix .cpp, $(basename $@)) -o $@

$(BASE_OBJS):
	$(CC) $(FLAG) -c $(addsuffix .cpp, $(basename $@)) -o $@

Base.o: $(BASE_OBJS)
	ld -r $(BASE_OBJS) -o ./Base.o

Http.o: $(HTTP_OBJS)
	ld -r $(HTTP_OBJS) -o ./Http.o

main: $(OBJS) Http.o Base.o Logger.o
	$(CC) $(FLAG) $(OBJS) Base.o Http.o Logger.o -lm -o $(MAIN)

BaseTest.o:
	$(CC) $(FLAG) -c BaseTest.cpp -o BaseTest.o

BaseTest: Base.o Logger.o BaseTest.o Httpd.o
	$(CC) $(FLAG) Base.o BaseTest.o Httpd.o Logger.o -lm -o BaseTest

clean:
	make -C logging clean
	-rm *.o $(BASE_TEST) $(HTTP_TEST)
	-rm http/*.o
	-rm base/*.o
