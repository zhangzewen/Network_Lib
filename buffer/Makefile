OBJECTS=$(patsubst %.cc, %.o, $(wildcard *.cc))
CC=g++
CFLAGS := -g3 -W -Werror -Wall -O0 -I../util/
LDFLAGS := -pthread
PWD=$(shell pwd)
INCLUDE=-I$(PWD)



all: $(OBJECTS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	rm -fr *.o
	rm -fr *.a
