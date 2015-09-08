OBJECTS=$(patsubst %.cc, %.o, $(wildcard *.cc))
CC=g++
CFLAGS+=-g3 -W -Werror -Wall
LDFLAGS+= -pthread
LIBNAME=util
LIB_NAME=lib$(LIBNAME).a
PWD=$(shell pwd)
INCLUDE=-I$(PWD)



all: $(OBJECTS)
#	ar rcs $(LIB_NAME) $(OBJECTS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LDFLAGS) 

clean:
	rm -fr *.o
	rm -fr *.a
