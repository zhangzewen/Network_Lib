OBJECTS=$(patsubst %.cc, %.o, $(wildcard *.cc))
CC=g++
CFLAGS := -g3 -W -Werror -Wall -O0 -I./poll/
LDFLAGS := -pthread -lboost_atomic -lboost_chrono -lboost_context -lboost_locale -lboost_regex -lboost_serialization
PWD=$(shell pwd)
INCLUDE=-I$(PWD)



all: $(OBJECTS)

%.o: %.cc
	$(CC) $(CFLAGS) -c $< $(LDFLAGS) -o $@

clean:
	rm -fr *.o
	rm -fr *.a
