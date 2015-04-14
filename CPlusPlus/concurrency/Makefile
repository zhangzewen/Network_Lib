OBJECTS=$(patsubst %.cc, %.o, $(wildcard *.cc))
CC=g++
CFLAGS+=-g3 -W -Werror -std=c++11
LDFLAGS+= -pthread
PWD=$(shell pwd)
TEST_DIR=$(PWD)/test
INCLUDE=-I. -I../util/



all: $(OBJECTS)

%.o: %.cc
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@ $(LDFLAGS) 

clean:
	@-rm -fr *.o
	@-rm -fr *.a
