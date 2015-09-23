OBJECTS=$(patsubst %.cc, %.o, $(wildcard *.cc))
CC=g++
CFLAGS := -g3 -W -Werror -Wall -O0 -I../
LDFLAGS := -pthread -lgtest -lgtest_main
PWD=$(shell pwd)
INCLUDE=-I$(PWD)
UTIL_OBJS := $(patsubst %.cc, %.o, $(wildcard ../*.cc))



all: $(OBJECTS) stripTest

%.o: %.cc
	$(CC) $(CFLAGS) -c $< $(LDFLAGS) -o $@

stripTest: stripTest.o $(UTIL_OBJS)
	$(CC) $(UTIL_OBJS) $(LDFLAGS) -o stripTest 

clean:
	rm -fr *.o
	rm -fr *.a
	rm -fr stripTest
