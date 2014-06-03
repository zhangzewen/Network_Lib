OBJECTS=$(patsubst %.c, %.o, $(wildcard *.c))
CC=gcc
CFLAGS+=-g3 -W -Werror
LDFLAGS+= -pthread
LIBNAME=util
LIB_NAME=lib$(LIBNAME).a
PWD=$(shell pwd)
TEST_DIR=$(PWD)/test
INCLUDE=-I$(PWD)



all: $(OBJECTS)
	ar rcs $(LIB_NAME) $(OBJECTS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LDFLAGS) 

clean:
	rm -fr *.o
	rm -fr *.a
